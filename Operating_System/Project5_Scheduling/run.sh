make fcfs
make sjf
make priority
make rr
make priority_rr
echo ""
echo "---------------------fcfs------------------------"
./fcfs schedule.txt
echo ""
echo "---------------------sjf-------------------------"
./sjf schedule.txt
echo ""
echo "-------------------priority----------------------"
./priority schedule.txt
echo ""
echo "----------------------rr-------------------------"
./rr schedule.txt
echo ""
echo "------------------priority_rr--------------------"
./priority_rr schedule.txt
