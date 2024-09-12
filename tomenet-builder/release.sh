#!/bin/bash

REPO="refracta/tomenet-ko"
SPLIT_SIZE="1GiB"
KEEP_FILES=false
SAME_OWNER=false

while [[ "$#" -gt 0 ]]; do
    case $1 in
        download)
            ACTION="download"
            ;;
        upload)
            ACTION="upload"
            ;;
        delete)
            ACTION="delete"
            ;;
        -r|--repo)
            REPO="$2"
            shift
            ;;
        -n|--name)
            NAME="$2"
            shift
            ;;
        -v|--version)
            VERSION="$2"
            shift
            ;;
        -p|--path)
            PATH_DIR="$2"
            shift
            ;;
        -t|--tag)
            TAG="$2"
            shift
            ;;
        -T|--title)
            TITLE="$2"
            shift
            ;;
        -s|--split-size)
            SPLIT_SIZE="$2"
            shift
            ;;
        -b|--branch)
            BRANCH="$2"
            shift
            ;;
        -k|--keep)
            KEEP_FILES=true
            ;;
        -l|--last)
            LAST="$2"
            shift
            ;;
        -o|--same-owner)
            SAME_OWNER=true
            ;;
        *)
            echo "Unknown parameter: $1"
            exit 1
            ;;
    esac
    shift
done

get_latest_tag() {
    local name=$1
    curl -s "https://api.github.com/repos/$REPO/tags" | jq -r ".[].name" | grep "^${name}-" | sort -rV | head -n 1
}

get_release_info() {
    if [ -n "$TAG" ]; then
        curl -s "https://api.github.com/repos/$REPO/releases/tags/$TAG"
    elif [ -n "$NAME" ] && [ -n "$VERSION" ]; then
        curl -s "https://api.github.com/repos/$REPO/releases/tags/${NAME}-${VERSION}"
    elif [ -n "$NAME" ]; then
        tags=$(curl -s "https://api.github.com/repos/$REPO/tags" | jq -r ".[].name" | grep "^${NAME}-" | sort -rV)
        for latest_tag in $tags; do
            release_info=$(curl -s "https://api.github.com/repos/$REPO/releases/tags/$latest_tag")
            release_title=$(echo $release_info | jq -r '.name')
            if [[ "$release_title" != "[Uploading]"* ]]; then
                echo "$release_info"
                return
            fi
        done
        echo "No suitable releases found for name prefix: $NAME"
        exit 1
    else
        echo "Either tag (-t) or name (-n) must be specified."
        exit 1
    fi
}

download_files() {
    mkdir -p "$PATH_DIR"

    release_info=$(get_release_info)
    release_tag=$(echo $release_info | jq -r '.tag_name')
    release_title=$(echo $release_info | jq -r '.name')

    if [ -z "$release_tag" ]; then
        echo "No valid release found. Exiting."
        exit 1
    fi

    echo "Release: $release_title ($release_tag)"

    assets=$(echo $release_info | jq -r '.assets[] | select(.name | test("^binary_")) | .browser_download_url')

    if [ -z "$assets" ]; then
        echo "No assets to download for the release $release_title ($release_tag). Exiting."
        exit 1
    fi

    for url in $assets; do
        echo "Downloading $url"
        curl -L -o "$PATH_DIR/$(basename $url)" $url
    done

    if ! cat "$PATH_DIR/binary_"* > "$PATH_DIR/binary.tar.gz"; then
        echo "Failed to concatenate downloaded parts. Exiting."
        exit 1
    fi

    rm "$PATH_DIR/binary_"*

    if [ "$SAME_OWNER" = true ]; then
        tar -xvzf "$PATH_DIR/binary.tar.gz" -C "$PATH_DIR" --same-owner
    else
        tar -xvzf "$PATH_DIR/binary.tar.gz" -C "$PATH_DIR"
    fi

    if [ "$KEEP_FILES" = false ]; then
        rm "$PATH_DIR/binary.tar.gz"
    fi

    echo "Download and extraction completed!"
}

upload_files() {
    FILE_PATH="$PATH_DIR"

    if [ ! -f "$FILE_PATH" ]; then
        echo "Specified path is not a file: $FILE_PATH"
        exit 1
    fi

    if [ -z "$TAG" ]; then
        if [ -n "$NAME" ] && [ -n "$VERSION" ]; then
            tag="${NAME}-${VERSION}"
        else
            tag="${NAME}-$(date -u +%Y%m%d%H%M)"
        fi
    else
        tag="$TAG"
    fi

    title=${TITLE:-$tag}
    uploading_title="[Uploading] $title"

    if [ -n "$BRANCH" ]; then
        gh release create "$tag" -R "$REPO" --title "$uploading_title" --target "$BRANCH" -n ""
    else
        gh release create "$tag" -R "$REPO" --title "$uploading_title" -n ""
    fi

    gh release upload "$tag" "$FILE_PATH" -R "$REPO"

    echo "Uploaded files. Updating release title..."

    gh release edit "$tag" -R "$REPO" --title "$title"

    echo "Upload and release update completed!"
}

delete_releases() {
    if [ -z "$NAME" ]; then
        echo "Name (-n) is required for delete action."
        exit 1
    fi

    releases=$(curl -s "https://api.github.com/repos/$REPO/releases" | jq -r ".[] | select(.tag_name | startswith(\"$NAME\")) | .tag_name")
    tags=($(echo "$releases" | sort -rV))
    if [ -n "$LAST" ]; then
        tags=(${tags[@]:$LAST})
    fi

    for tag in "${tags[@]}"; do
        echo "Deleting release and tag: $tag"
        gh release delete "$tag" -R "$REPO" -y
        gh api -X DELETE "repos/$REPO/git/refs/tags/$tag"
    done

    echo "Deletion completed!"
}

if [ "$ACTION" == "download" ]; then
    if [ -z "$PATH_DIR" ] || [ -z "$NAME" ]; then
        echo "Path (-p) and name (-n) are required for download action."
        exit 1
    fi
    download_files
elif [ "$ACTION" == "upload" ]; then
    if [ -z "$PATH_DIR" ] || [ -z "$NAME" ]; then
        echo "Path (-p) and name (-n) are required for upload action."
        exit 1
    fi
    upload_files
elif [ "$ACTION" == "delete" ]; then
    delete_releases
else
    echo "Action (download/upload/delete) is required."
    exit 1
fi
