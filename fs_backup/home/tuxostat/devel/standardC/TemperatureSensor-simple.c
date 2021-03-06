// - TemperatureSensor simple -
// This simple example simple opens a temperature sensor phidget and waits for one to be attached.  The program will then wait for
// user input to terminate.  While waiting it will display the data generated by the events, such as the temperature change event
// which will display the currently measured temperature without sensitivity modifications.  For a more detailed example where
// sensitivity can be adjusted and all the functionalities of a temperature sensor are demonstrated, see TemperatureSensor-full 
// example.
// 
// Please note that this example was designed to work with only one Phidget TemperatureSensor connected. For an example using multiple 
// Phidget TemperatureSensors, please see a "multiple" example in the TemperatureSensor Examples folder.
//
// Copyright 2007 Phidgets Inc.  All rights reserved.


#include <stdio.h>
#include <phidget21.h>

int AttachHandler(CPhidgetHandle TEMP, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (TEMP, &name);
	CPhidget_getSerialNumber(TEMP, &serialNo);
	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int DetachHandler(CPhidgetHandle TEMP, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (TEMP, &name);
	CPhidget_getSerialNumber(TEMP, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int ErrorHandler(CPhidgetHandle TEMP, void *userptr, int ErrorCode, const char *Description)
{
	printf("Error handled. %i - %s\n", ErrorCode, Description);
	return 0;
}

int TemperatureChangeHandler(CPhidgetTemperatureSensorHandle TEMP, void *usrptr, int Index, double Value)
{
	printf("Temperature sensor: %d > Temperature: %f\n", Index, Value);
	return 0;
}

//Display the properties of the attached phidget to the screen.  We will be displaying the name, serial number and version of the attached device.
//We will also display the total number of available LEDs
int display_properties(CPhidgetTemperatureSensorHandle phid)
{
	int serialNo, version, numInputs, i;
	double value;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetTemperatureSensor_getNumTemperatureInputs (phid, &numInputs);
	

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Temperature Inputs: %d\n", numInputs);

	for(i = 0; i < numInputs; i++)
	{
		CPhidgetTemperatureSensor_getTemperatureChangeTrigger (phid, i, &value);
		printf("Temperature Input #: %d > sensitivity: %f\n", i, value);
	}

	return 0;
}

int tempsensor_simple()
{
	int result;
	const char *err;

	//Declare an temperature sensor handle
	CPhidgetTemperatureSensorHandle temp = 0;

	//create the accelerometer object
	CPhidgetTemperatureSensor_create(&temp);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)temp, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)temp, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)temp, ErrorHandler, NULL);

	//Registers a callback that will run if the Temperature changes by more than the Temperature trigger.
	//Requires the handle for the Temperature Sensor, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetTemperatureSensor_set_OnTemperatureChange_Handler(temp, TemperatureChangeHandler, NULL);

	//open the temperature sensor for device connections
	CPhidget_open((CPhidgetHandle)temp, -1);

	//get the program to wait for an temperature sensor device to be attached
	printf("Waiting for TemperatureSensor to be attached....");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)temp, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached accelerometer device
	display_properties(temp);

	//read led event data
	printf("Reading.....\n");

	//keep displaying temperature sensor event data until user input is read

	//modify the sensor sensitivity, index 1 is the thermocouple sensor, index 0 is the onboard or ambient sensor
	printf("Setting sensitivity of the thermocouple to 2.00. Press any key to continue\n");
	getchar();

	CPhidgetTemperatureSensor_setTemperatureChangeTrigger (temp, 1, 2.00);

	printf("Press any key to end\n");
	getchar();

	//since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
	printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)temp);
	CPhidget_delete((CPhidgetHandle)temp);

	//all done, exit
	return 0;
}

int main(int argc, char* argv[])
{
	tempsensor_simple();
	return 0;
}

