#!/bin/bash
##################################################
#  Filename:  install.sh
#  By:  Matthew Evans
#  See LICENSE.md for copyright information.
##################################################
#  Script to install wtengine tools
#
#  Creates symbolic links to scripts
#  Run with --uninstall to remove links instead
#  Run with --ask to confirm each file
##################################################

##################################################
#  Script variables
##################################################
#  Default folder to install to
DEFAULT_INSTALL_LOCATION="/usr/local/bin"
#  Filenames to make symlinks to
SCRIPT_FILE_NAMES=("csv2sdf/csv2sdf.sh")

##################################################
#  Function to create a confirmation prompt
#  First argument used as display text
#  Accepts ENTER, "Y" or "y" then returns true
#  All other input returns false
##################################################
confirm_prompt()
{
    read -p "$1 [Y/n]? " CONFIRM_RESULT
    CONFIRM_RESULT=${CONFIRM_RESULT:-true}
    if [ "$CONFIRM_RESULT" = true -o "$CONFIRM_RESULT" = "Y" -o "$CONFIRM_RESULT" = "y" ]; then
        true
        return
    fi
    false
    return
}

##################################################
#  Parse arguments
##################################################
RUN_UNINSTALL="false"
ASK_EACH_FILE="false"
for ARGS in "$@"; do
    #  Check if uninstall flag passed
    if [ "$ARGS" = "--uninstall" ]; then
        RUN_UNINSTALL="true"
    fi
    #  Check if ask flag passed
    if [ "$ARGS" = "--ask" ]; then
        ASK_EACH_FILE="true"
    fi
done

##################################################
#  Start main script
##################################################
echo
echo "*** WTEngine tools management ***"
echo

#  Get install location
read -p "Install location [$DEFAULT_INSTALL_LOCATION]: " INSTALL_LOCATION
#  If install location null, use default
INSTALL_LOCATION=${INSTALL_LOCATION:-$DEFAULT_INSTALL_LOCATION}
echo

#  Get the scripts' location
CURRENT_DIR="$( cd "$( dirname "$0" )" && pwd )"

##########################
#  Run uninstall
##########################
if [ "$RUN_UNINSTALL" = true ]; then
    echo "Removing scripts from '$INSTALL_LOCATION'..."
    #  Iterate over script filenames, deleting symlinks
    for FILENAME in "${SCRIPT_FILE_NAMES[@]}"; do
        SIMPLENAME="${FILENAME##*/}"
        if [ "$ASK_EACH_FILE" = false ] || (confirm_prompt "Do you want to remove '${SIMPLENAME%.*}' from '$INSTALL_LOCATION'"); then
            rm $INSTALL_LOCATION/${SIMPLENAME%.*}
        fi
    done
##########################
#  Run install
##########################
else
    echo "Installing scripts to '$INSTALL_LOCATION'..."

    #  Store the location of the tools install folder
    USER_HOME=$(getent passwd $SUDO_USER | cut -d: -f6)
    if [ ! -d "$USER_HOME/.wtengine" ]; then
        mkdir "$USER_HOME/.wtengine"
    fi
    echo "$CURRENT_DIR" > "$USER_HOME/.wtengine/tools_location"

    #  Iterate over script filenames, creating symlinks for each
    for FILENAME in "${SCRIPT_FILE_NAMES[@]}"; do
        SIMPLENAME="${FILENAME##*/}"
        if [ "$ASK_EACH_FILE" = false ] || (confirm_prompt "Do you want to install '${SIMPLENAME%.*}' to '$INSTALL_LOCATION'"); then
            if [ -e "$INSTALL_LOCATION/${SIMPLENAME%.*}" ]; then
                rm "$INSTALL_LOCATION/${SIMPLENAME%.*}"
            fi
            ln -s $CURRENT_DIR/$FILENAME $INSTALL_LOCATION/${SIMPLENAME%.*}
            chmod u+x $INSTALL_LOCATION/${SIMPLENAME%.*}
        fi
    done
fi

echo "Done!"
echo
