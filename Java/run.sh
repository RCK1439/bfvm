
echo "compiling..."
javac -d bin src/*.java

echo "running $1"
time java -cp bin BFVM $1