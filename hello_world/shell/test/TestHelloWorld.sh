sh ./HelloWorld.sh | grep 'Hello World from Shell!' &> /dev/null
if [ $? == 0 ]; then
   echo "matched"
   exit 0
else
   exit 1
fi