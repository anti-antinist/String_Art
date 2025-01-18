rm -rf build/
cmake -B build/
cd build
rm ../../.vscode/compile_commands.json
cp compile_commands.json ../../.vscode/
make
time ./Str_Art $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}