#define NO_WIN32_LEAN_AND_MEAN
#include "MinosControl_pch.h"
#pragma hdrstop

#include <setupapi.h>
#include <dbt.h>
extern "C" {
#include "hidsdi.h"	/* note: part of Windows DDK */
#include "hidpi.h"	/* note: also part of Windows DDK */
}

#include "HIDControl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

THidControlFM *HidControlFM = 0;
//---------------------------------------------------------------------------
__fastcall THidControlFM::THidControlFM(TComponent* Owner)
	: TForm(Owner), HidDevHandle(INVALID_HANDLE_VALUE),
	HidAttached(false), hDevNotify(0)
{
}
__fastcall THidControlFM::~THidControlFM()
{
trace("THidControlFM::~THidControlFM");
	if (HidDevHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(HidDevHandle);
	}
}
/**********************************************************
*
*	Function: bOpenHidDevice
*	Purpose: tries to open a HID device based on VID and PID
*	Parameters: vid - HID device's vendor ID
*				pid - HID device's product ID
*				HidDevHandle - pointer to a handle to the HID device
*	Returns: TRUE, if device is found
*			 FALSE, if device is not found
*
**********************************************************/

bool THidControlFM::bOpenHidDevice( USHORT vid, USHORT pid)
{
static GUID HidGuid;						/* HID Globally Unique ID: windows supplies us with this value */
HDEVINFO HidDevInfo;						/* handle to structure containing all attached HID Device information */
SP_DEVICE_INTERFACE_DATA devInfoData;		/* Information structure for HID devices */
BOOLEAN Result;								/* result of getting next device information structure */
DWORD Index;								/* index of HidDevInfo array entry */
DWORD DataSize;								/* size of the DeviceInterfaceDetail structure */
BOOLEAN GotRequiredSize;					/* 1-shot got device info data structure size flag */
PSP_DEVICE_INTERFACE_DETAIL_DATA detailData;/* device info data */
DWORD RequiredSize;							/* size of device info data structure */
//BOOLEAN DIDResult;							/* get device info data result */
HIDD_ATTRIBUTES HIDAttrib;					/* HID device attributes */

trace("THidControlFM::bOpenHidDevice");
				/* initialize variables */
				GotRequiredSize = FALSE;



				/* 1) Get the HID Globally Unique ID from the OS */
				HidD_GetHidGuid(&HidGuid);


				/* 2) Get an array of structures containing information about
				all attached and enumerated HIDs */
				HidDevInfo = SetupDiGetClassDevs(	&HidGuid,
													NULL,
													NULL,
													DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);


				/* 3) Step through the attached device list 1 by 1 and examine
				each of the attached devices.  When there are no more entries in
				the array of structures, the function will return FALSE. */

				Index = 0;									/* init to first index of array */
				devInfoData.cbSize = sizeof(devInfoData);	/* set to the size of the structure
															that will contain the device info data */

				do {
					/* Get information about the HID device with the 'Index' array entry */
					Result = SetupDiEnumDeviceInterfaces(	HidDevInfo,
															0,
															&HidGuid,
															Index,
															&devInfoData);

					/* If we run into this condition, then there are no more entries
					to examine, we might as well return FALSE at point */
					if(Result == FALSE)
					{
						/* free the memory allocated for DetailData */
						if(detailData != NULL)
							free(detailData);

						/* free HID device info list resources */
						SetupDiDestroyDeviceInfoList(HidDevInfo);

						return FALSE;
					}


					if(GotRequiredSize == FALSE)
					{
						/* 3) Get the size of the DEVICE_INTERFACE_DETAIL_DATA
						structure.  The first call will return an error condition,
						but we'll get the size of the strucure */
						/*bool DIDResult = */SetupDiGetDeviceInterfaceDetail(	HidDevInfo,
																	&devInfoData,
																	NULL,
																	0,
																	&DataSize,
																	NULL);
						GotRequiredSize = TRUE;

						/* allocate memory for the HidDevInfo structure */
						detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(DataSize);

						/* set the size parameter of the structure */
						detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
					}


					/* 4) Now call the function with the correct size parameter.  This
					function will return data from one of the array members that
					Step #2 pointed to.  This way we can start to identify the
					attributes of particular HID devices.  */
					/*bool DIDResult =*/ SetupDiGetDeviceInterfaceDetail(	HidDevInfo,
																&devInfoData,
																detailData,
																DataSize,
																&RequiredSize,
																NULL);


					/* 5) Open a file handle to the device.  Make sure the
					attibutes specify overlapped transactions or the IN
					transaction may block the input thread. */
					HidDevHandle = CreateFile( detailData->DevicePath,
												GENERIC_READ | GENERIC_WRITE,
												FILE_SHARE_READ | FILE_SHARE_WRITE,
												NULL,
												OPEN_EXISTING,
												/*FILE_FLAG_OVERLAPPED*/0,
												NULL);


					/* 6) Get the Device VID & PID to see if it's the device we want */
					if(HidDevHandle != INVALID_HANDLE_VALUE)
					{
						HIDAttrib.Size = sizeof(HIDAttrib);
						HidD_GetAttributes(	HidDevHandle, &HIDAttrib);

						if((HIDAttrib.VendorID == vid) && (HIDAttrib.ProductID == pid))
						{
							/* free the memory allocated for DetailData */
							if(detailData != NULL)
								free(detailData);

							/* free HID device info list resources */
							SetupDiDestroyDeviceInfoList(HidDevInfo);

							return TRUE;	/* found HID device */
						}

						/* 7) Close the Device Handle because we didn't find the device
						with the correct VID and PID */
						CloseHandle(HidDevHandle);
					}

					Index++;	/* increment the array index to search the next entry */

				} while(Result == TRUE);

	/* free the memory allocated for DetailData */
	if(detailData != NULL)
		free(detailData);

	/* free HID device info list resources */
	SetupDiDestroyDeviceInfoList(HidDevInfo);

	return FALSE;

}


/**********************************************************
*
*	Function: bHidDeviceNotify
*	Purpose: Sets up the HID device notification events.  The
*			message contains the events
*			DBT_DEVICEARRIVAL and DBT_DEVICEREMOVALCOMPLETE.  It
*			is then up to the application to find out if the
*			event is refering to the device it is connected with by
*			parsing a structure that the event points to.
*	Parameters: hWnd - handle to window that notifications should be
*				sent to.
*	Returns: TRUE, if device notification is set up
*			 FALSE, if device notification setup fails
*
**********************************************************/

bool THidControlFM::bHidDeviceNotify()
{
trace("THidControlFM::bHidDeviceNotify");
	GUID HidGuid;										/* temporarily stores Windows HID Class GUID */
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;	/* un/plug notification filter */

	HWND hWnd = Handle;

	/* Set up device notification (i.e. plug or unplug of HID Devices) */

	/* 1) get the HID GUID */
	HidD_GetHidGuid(&HidGuid);

	/* 2) clear the notification filter */
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter));

	/* 3) assign the previously cleared structure with the correct data
	so that the application is notified of HID device un/plug events */
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_classguid = HidGuid;



	/* 4) register device notifications for this application */
	hDevNotify = RegisterDeviceNotification(	hWnd,
												&NotificationFilter,
												DEVICE_NOTIFY_WINDOW_HANDLE );

	/* 5) notify the calling procedure if the HID device will not be recognized */
	if(!hDevNotify)
		return FALSE;

	return TRUE;

}
bool THidControlFM::ReadFile(char *Report, unsigned long ToRead, unsigned long &BytesRead)
{
  bool Result = false;
  if (HidDevHandle != INVALID_HANDLE_VALUE)
  {
	 Result = ::ReadFile(HidDevHandle, Report, ToRead, &BytesRead, 0);
  }
  return Result;
}
bool THidControlFM::WriteFile(char *Report, unsigned long ToWrite, unsigned long &BytesWritten)
{
  bool Result = false;
  if (HidDevHandle != INVALID_HANDLE_VALUE)
  {
	 Result = ::WriteFile(HidDevHandle, Report, ToWrite, &BytesWritten, 0);
  }
  return Result;
}
//---------------------------------------------------------------------------
/* global constants */
#define VID 0x10CF
#define PID 0x5500
void __fastcall THidControlFM::WmDeviceChange( TMessage &Msg )
{
trace("THidControlFM::WmDeviceChange");
	/* try to open a handle to the device */
	if (HidDevHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(HidDevHandle);
	}

	HidAttached = bOpenHidDevice(VID, PID);


	/* take the appropriate action for the message */
	switch(LOWORD(Msg.WParam)) {

		/* HID device arrival */
		case DBT_DEVICEARRIVAL:
		{
			break;
		}

		/* HID device removal */
		case DBT_DEVICEREMOVECOMPLETE:
		{
			break;
		}


		/* HID Device Change - Note that Win2k tends to use this messages
		instead of the previous two. */
		case DBT_DEVNODES_CHANGED:
		{

			break;
		}



		default:
			break;
	}

}
//---------------------------------------------------------------------------
void __fastcall THidControlFM::FormShow(TObject */*Sender*/)
{
trace("THidControlFM::FormShow");
	/* Set up notifications of HID device attach and detach events */
	if( bHidDeviceNotify() == FALSE)
		MessageBox(Handle, "Failed to Register Device Notifications", "ERROR", MB_ICONSTOP);

	HidAttached = bOpenHidDevice(VID, PID);
}
//---------------------------------------------------------------------------
