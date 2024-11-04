#!/bin/sh

CLIENT_DIR="clientSide"
SERVER_DIR="serverSide"

show_usage() {
    echo "Usage: $0 [OPTION] [SERIAL_PORT]"
    echo "Build and manage client and server projects."
    echo
    echo "Options:"
    echo "  client        Build only the client project."
    echo "  server        Build only the server project (includes upload target)."
    echo "  clean         Remove build directories for both client and server."
    echo "  -h, --help    Display this help and exit."
    echo
    echo "If no option is specified, both client and server will be built."
    echo "To specify a custom serial port for the server, pass it as an additional argument."
}

build_project() {
    local project_dir=$1
    local is_server=$2
    local serial_port=$3

    echo "Building project in $project_dir..."

    mkdir -p "$project_dir/build"

    cd "$project_dir/build" || exit

    if [ "$is_server" = "true" ]; then
        cmake -DSERIAL_PORT_PATH="$serial_port" ..
    else
        cmake ..
    fi
    make

    if [ "$is_server" = "true" ]; then
        cmake --build . --target upload
    fi

    cd - > /dev/null
}

clean_build() {
    echo "Cleaning build directories..."
    rm -rf "$CLIENT_DIR/build" "$SERVER_DIR/build"
    echo "Build directories cleaned."
}

case "$1" in
    clean)
        clean_build
        exit 0
        ;;
    client)
        build_project "$CLIENT_DIR" "false"
        ;;
    server)
        serial_port=${2:-"/dev/ttyUSB0"} # Default to "/dev/ttyUSB0" if not provided
        build_project "$SERVER_DIR" "true" "$serial_port"
        ;;
    -h|--help)
        show_usage
        exit 0
        ;;
    *)
        if [ -n "$1" ]; then
            echo "Invalid option: $1"
            show_usage
            exit 1
        else
            echo "No specific argument provided, building both client and server..."
            build_project "$CLIENT_DIR" "false"
            build_project "$SERVER_DIR" "true" "/dev/ttyUSB0"
        fi
        ;;
esac

echo "Build process completed."

