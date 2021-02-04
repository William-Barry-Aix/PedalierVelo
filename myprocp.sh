#! /bin/bash
if (($# != 2)); then
echo "Usage: $0 old_rep new_rep" >&2
exit 1
fi
old_rep=$1 ; p=${old_rep%/} ; old_pro=${p##*/}
new_rep=$2 ; p=${new_rep%/} ; new_pro=${p##*/}
echo "Copying $old_rep -> $new_rep ..."
cp -a "$old_rep" "$new_rep" || return 1
echo "Replacing $old_pro -> $new_pro ..."
sed -i "s/$old_pro/$new_pro/g" "$new_rep"/*
mv "$new_rep/$old_pro.pro" "$new_rep/$new_pro.pro"
test -f "$new_rep/$old_pro.qrc" &&
mv "$new_rep/$old_pro.qrc" "$new_rep/$new_pro.qrc"
rm -f "$new_rep/$old_pro.pro"* "$new_rep"/*[~%]
echo "Done"