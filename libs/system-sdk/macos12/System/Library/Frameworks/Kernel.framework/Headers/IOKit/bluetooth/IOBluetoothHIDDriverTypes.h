/*
	File:		IOBluetoothHIDDriverTypes.h
	Contains:	Type defined for the IOBluetoothHIDDriver.h.
	Copyright:	(c) 2001-2008 by Apple, all rights reserved.
*/


#ifndef IOBLUETOOTHHIDDRIVERTYPES_H
#define IOBLUETOOTHHIDDRIVERTYPES_H

// Vendor ID Sources
//------------------
#define kVendorIDSourceBluetoothSIG				0x1
#define kVendorIDSourceUSBIF					0x2

// Bluetooth HID Transaction Headers
//----------------------------------
#define IOBTHID_HANDSHAKE_HEADER				0x00
#define IOBTHID_HID_CONTROL_HEADER				0x10
#define IOBTHID_GET_REPORT_HEADER				0x40
#define IOBTHID_SET_REPORT_HEADER				0x50
#define IOBTHID_GET_PROTOCOL_HEADER				0x60
#define IOBTHID_SET_PROTOCOL_HEADER				0x70
#define IOBTHID_GET_IDLE_HEADER					0x80
#define IOBTHID_SET_IDLE_HEADER					0x90
#define IOBTHID_DATA_HEADER						0xA0
#define IOBTHID_DATC_HEADER						0xB0

// Handshake Types
//----------------
#define IOBTHID_HANDSHAKE_SUCCESSFUL			0x0
#define IOBTHID_HANDSHAKE_NOT_READY				0x1
#define IOBTHID_HANDSHAKE_INVALID_REPORT_ID		0x2
#define IOBTHID_HANDSHAKE_UNSUPPORTED_REQUEST	0x3
#define IOBTHID_HANDSHAKE_INVALID_PARAMETER		0x4
#define IOBTHID_HANDSHAKE_ERR_UNKNOWN			0xD
#define IOBTHID_HANDSHAKE_ERR_FATAL				0xF

// HID_Control Types
//------------------
#define IOBTHID_CONTROL_NOP						0x0
#define IOBTHID_CONTROL_HARD_RESET				0x1
#define IOBTHID_CONTROL_SOFT_RESET				0x2
#define IOBTHID_CONTROL_SUSPEND					0x3
#define IOBTHID_CONTROL_EXIT_SUSPEND			0x4
#define IOBTHID_CONTROL_VC_UNPLUG				0x5

// Protocol Types
//---------------
#define IOBTHID_BOOT_PROTOCOL					0x0
#define IOBTHID_REPORT_PROTOCOL					0x1

// Report Types
//-------------
#define IOBTHID_RESERVED_REPORT					0x0
#define IOBTHID_OTHER_REPORT					0x0
#define IOBTHID_INPUT_REPORT					0x1
#define IOBTHID_OUTPUT_REPORT					0x2
#define IOBTHID_FEATURE_REPORT					0x3

#endif // IOBLUETOOTHHIDDRIVERTYPES_H
