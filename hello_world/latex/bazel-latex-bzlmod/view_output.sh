#!/bin/sh
filename="$1"
stderr_type="$2"

if type xdg-open > /dev/null 2>&1; then
    # X11-based systems (Linux, BSD).
    
    if [ "$stderr_type" = "None" ] ; then
        exec xdg-open "${filename}" 2>/dev/null &
    else
        exec xdg-open "${filename}" &
    fi

elif type open > /dev/null 2>&1; then
    # macOS.
    exec open "${filename}"
else
    echo "Don't know how to view PDFs on this platform." >&2
    exit 1
fi
