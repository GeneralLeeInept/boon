#include "vga.h"

#include "port.h"

namespace vga
{
static constexpr uint16_t AttributeControllerWriteIndexData = 0x3C0;
static constexpr uint16_t AttributeControllerReadData = 0x3C1;

static constexpr uint16_t MiscOutputWrite = 0x3C2;
static constexpr uint16_t MiscOutputRead = 0x3CC;

static constexpr uint16_t SequencerIndex = 0x3C4;
static constexpr uint16_t SequencerData = 0x3C5;

static constexpr uint16_t GraphicsControllerIndex = 0x3CE;
static constexpr uint16_t GraphicsControllerData = 0x3CF;

static constexpr uint16_t DACMask = 0x3C6; // Should normally be set to 0xFF
static constexpr uint16_t DACIndexForReadData = 0x3C7;
static constexpr uint16_t DACIndexForWriteData = 0x3C8;
static constexpr uint16_t DACData = 0x3C9;

static constexpr uint16_t FeatureControlRead = 0x3CA;
static constexpr uint16_t FeatureControlWrite = 0x3DA;

static constexpr uint16_t CRTControllerIndex = 0x3D4; // If Bit 0 of MiscellaneousOutput set, this is 0x3B4, [Registers 0 to 7] protected by [Bit 7 of Reg 17 (0x11)]
static constexpr uint16_t CRTControllerData = 0x3D5;

static constexpr uint32_t VramBase = 0xA0000;

void Init()
{
}

void SetModeX()
{
#if 0
    // Disable output
    outw(SequencerIndex, 0x0100);

    // Misc output - CRT controller index @ 0x3D4
    outb(MiscOutputWrite, 0xE3);

    outb(FeatureControlWrite, 0x00);    // Make sure attribute controller is ready to accept an index

    // Unlock CRTC registers
    outb(CRTControllerIndex, 0x11);
    uint8_t verticalRetraceEnd = inb(CRTControllerData);
    verticalRetraceEnd &= 0x7F; // Remove protection flag
    outb(CRTControllerData, verticalRetraceEnd);

    // Set registers    
    outb(AttributeControllerWriteIndexData, 0x10);
    outb(AttributeControllerWriteIndexData, 0x41);

    outb(AttributeControllerWriteIndexData, 0x11);
    outb(AttributeControllerWriteIndexData, 0x00);

    outb(AttributeControllerWriteIndexData, 0x12);
    outb(AttributeControllerWriteIndexData, 0x0F);

    outb(AttributeControllerWriteIndexData, 0x13);
    outb(AttributeControllerWriteIndexData, 0x00);

    outb(AttributeControllerWriteIndexData, 0x14);
    outb(AttributeControllerWriteIndexData, 0x00);

    outb(MiscOutputWrite, 0xE3);    // Redundant?

    outw(SequencerIndex, 0x0101);
    outw(SequencerIndex, 0x0003);
    outw(SequencerIndex, 0x0604);

    outw(GraphicsControllerIndex, 0x4005);
    outw(GraphicsControllerIndex, 0x0506);

    outw(CRTControllerIndex, 0x5F00);
    outw(CRTControllerIndex, 0x4F01);
    outw(CRTControllerIndex, 0x5002);
    outw(CRTControllerIndex, 0x8203);
    outw(CRTControllerIndex, 0x5404);
    outw(CRTControllerIndex, 0x8005);
    outw(CRTControllerIndex, 0x0D06);
    outw(CRTControllerIndex, 0x3E07);
    outw(CRTControllerIndex, 0x0008);
    outw(CRTControllerIndex, 0x4109);
    outw(CRTControllerIndex, 0xEA10);
    outw(CRTControllerIndex, 0xAC11);
    outw(CRTControllerIndex, 0xDF12);
    outw(CRTControllerIndex, 0x2813);
    outw(CRTControllerIndex, 0x0014);
    outw(CRTControllerIndex, 0xE715);
    outw(CRTControllerIndex, 0x0616);
    outw(CRTControllerIndex, 0xE317);

    // Re-enable display
    outw(SequencerIndex, 0x0300);
#else
    outw(SequencerIndex, 0x0604);
    outw(SequencerIndex, 0x0100);
    outb(MiscOutputWrite, 0xe3);
    outw(SequencerIndex, 0x0300);
    outb(CRTControllerIndex, 0x11);
    uint8_t x = inb(CRTControllerData);
    outb(CRTControllerData, x & 0x7f);

    outw(CRTControllerIndex, 0x0d06);
    outw(CRTControllerIndex, 0x3e07);
    outw(CRTControllerIndex, 0x4109);
    outw(CRTControllerIndex, 0xea10);
    outw(CRTControllerIndex, 0xac11);
    outw(CRTControllerIndex, 0xdf12);
    outw(CRTControllerIndex, 0x0014);
    outw(CRTControllerIndex, 0xe715);
    outw(CRTControllerIndex, 0x0616);
    outw(CRTControllerIndex, 0xe317);

    outw(SequencerIndex, 0x0f02);
    outw(SequencerIndex, 0x0300);
#endif

    // Set palette
    outb(DACMask, 0xFF);
    outb(DACIndexForWriteData, 0);
    for (uint8_t i = 0; i < 255; i++) 
    {
        outb(DACData, (((i >> 5) & 0x7) * (256 / 8)) / 4);
        outb(DACData, (((i >> 2) & 0x7) * (256 / 8)) / 4);
        outb(DACData, (((i >> 0) & 0x3) * (256 / 4)) / 4);
    }

    // set color 255 = white
    outb(DACData, 0x3F);
    outb(DACData, 0x3F);
    outb(DACData, 0x3F);
}

void DrawTestPattern()
{
    uint32_t* mem = (uint32_t*)VramBase;
    for (uint8_t y = 0; y < 15; y++)
    {
        for (uint16_t x = 0; x < 80; x++)
        {
            mem[y * 80 + x] = 0xFFffFFff;
        }
    }
}
}