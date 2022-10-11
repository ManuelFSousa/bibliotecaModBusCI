client: ModbusAP.o ModbusTCP.o
	gcc clientM.c ModbusAP.o ModbusTCP.o -o clientM

ModbusAP.o: ModbusAP.c ModbusAP.h
	gcc -c ModbusAP.c

ModbusTCP.o: ModbusTCP.c ModbusTCP.h
	gcc -c ModbusTCP.c

clean:
	rm *.o clientM