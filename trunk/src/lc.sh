cat *.cpp *.h utils/*.cpp utils/*.h objects/*.cpp objects/*.h modes/*.cpp modes/*.h | rgrep -v "^[[:space:]]*$" | rgrep -v "[[:space:]]*//" | wc -
