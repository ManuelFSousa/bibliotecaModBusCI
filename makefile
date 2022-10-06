client: ModbusAP.o ModbusTCP.o
	gcc client.c ModbusAP.o ModbusTCP.o -o client

ModbusAP.o: ModbusAP.c ModbusAP.h
	gcc -c ModbusAP.c

ModbusTCP.o: ModbusTCP.c ModbusTCP.h
	gcc -c ModbusTCP.c

clean:
	rm *.o client