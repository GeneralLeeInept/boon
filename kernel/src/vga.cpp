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

static constexpr uint16_t FeatureControlRead = 0x3DA;
static constexpr uint16_t FeatureControlWrite = 0x3DA;

static constexpr uint16_t CrtControllerIndex = 0x3D4; // If Bit 0 of MiscellaneousOutput set, this is 0x3B4, [Registers 0 to 7] protected by [Bit 7 of Reg 17 (0x11)]
static constexpr uint16_t CrtControllerData = 0x3D5;

static constexpr int SequencerRegisterCount = 5;
static constexpr int CrtControllerRegisterCount = 25;
static constexpr int GraphicsControllerRegisterCount = 9;
static constexpr int AttributeControllerRegisterCount = 21;

static constexpr uint32_t VramBase = 0xA0000;

void Init()
{
}

static void SetupPallette()
{
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

static void SetupVideoMode(uint8_t* registers)
{
    // MISC
    outb(MiscOutputWrite, *registers);
    registers++;

    // SEQ
    for (uint8_t i = 0; i < SequencerRegisterCount; ++i)
    {
        outb(SequencerIndex, i);
        outb(SequencerData, *registers);
        registers++;
    }

    // CRTC
    // unlock CRTC registers
    outb(CrtControllerIndex, 0x03);
    uint8_t x = inb(CrtControllerData);
    outb(CrtControllerData, x | 0x80);
    outb(CrtControllerIndex, 0x11);
    x = inb(CrtControllerData);
    outb(CrtControllerData, x & ~0x80);
    // make sure they remain unlocked
    registers[0x03] |= 0x80;
    registers[0x11] &= ~0x80;

    for (uint8_t i  = 0; i < CrtControllerRegisterCount; ++i)
    {
        outb(CrtControllerIndex, i);
        outb(CrtControllerData, *registers);
        registers++;
    }

    // GC
    for (uint8_t i = 0; i < GraphicsControllerRegisterCount; ++i)
    {
        outb(GraphicsControllerIndex, i);
        outb(GraphicsControllerData, *registers);
        registers++;
    }

    // AC
    for (uint8_t i = 0; i < AttributeControllerRegisterCount; ++i)
    {
        // Make sure next AC write is an index
        (void)inb(FeatureControlRead);
        outb(AttributeControllerWriteIndexData, i);
        outb(AttributeControllerWriteIndexData, *registers);
        registers++;
    }

    (void)inb(FeatureControlRead);
    outb(AttributeControllerWriteIndexData, 0x20);
}

static uint32_t GetVramBaseAddr()
{
	uint32_t seg;
	outb(GraphicsControllerIndex, 6);
	seg = inb(GraphicsControllerData);
	seg >>= 2;
	seg &= 3;

	switch(seg)
	{
	case 0:
	case 1:
		seg = 0xA0000;
		break;
	case 2:
		seg = 0xB0000;
		break;
	case 3:
		seg = 0xB8000;
		break;
	}
	return seg;
}
// Set 320x200x256 colors, linear framebuffer - named 13H after the BIOS mode number
void SetMode13H()
{
    // Register settings from https://files.osdev.org/mirrors/geezer/osd/graphics/modes.c
    uint8_t registers[] =
    {
        /* MISC */
        0x63,
        /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
        0xFF,
        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00,	0x00
    };

    SetupVideoMode(registers);
    SetupPallette();
}

// 320x200x256 colors, planar
void SetModeY()
{
    static uint8_t registers[] =
    {
        /* MISC */
        0x63,
        /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x06,
        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3,
        0xFF,
        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,
        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00, 0x00
    };

    SetupVideoMode(registers);
    SetupPallette();
}

// Set custom VGA mode - 320x240x256 colors, planar
void SetModeX()
{
#if 0
    // Set ModeX directly.
    // Disable output
    outw(SequencerIndex, 0x0100);

    // Misc output - CRT controller index @ 0x3D4
    outb(MiscOutputWrite, 0xE3);

    outb(FeatureControlWrite, 0x00);    // Make sure attribute controller is ready to accept an index

    // Unlock CRTC registers
    outb(CrtControllerIndex, 0x11);
    uint8_t verticalRetraceEnd = inb(CrtControllerData);
    verticalRetraceEnd &= 0x7F; // Remove protection flag
    outb(CrtControllerData, verticalRetraceEnd);

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

    outw(CrtControllerIndex, 0x5F00);
    outw(CrtControllerIndex, 0x4F01);
    outw(CrtControllerIndex, 0x5002);
    outw(CrtControllerIndex, 0x8203);
    outw(CrtControllerIndex, 0x5404);
    outw(CrtControllerIndex, 0x8005);
    outw(CrtControllerIndex, 0x0D06);
    outw(CrtControllerIndex, 0x3E07);
    outw(CrtControllerIndex, 0x0008);
    outw(CrtControllerIndex, 0x4109);
    outw(CrtControllerIndex, 0xEA10);
    outw(CrtControllerIndex, 0xAC11);
    outw(CrtControllerIndex, 0xDF12);
    outw(CrtControllerIndex, 0x2813);
    outw(CrtControllerIndex, 0x0014);
    outw(CrtControllerIndex, 0xE715);
    outw(CrtControllerIndex, 0x0616);
    outw(CrtControllerIndex, 0xE317);

    // Re-enable output
    outw(SequencerIndex, 0x0300);
#else
    // Set VGA mode 13H then tweak settings to enable ModeX
    SetMode13H();
    
    outw(SequencerIndex, 0x0604);
    outw(SequencerIndex, 0x0100);
    outb(MiscOutputWrite, 0xe3);
    outw(SequencerIndex, 0x0300);
    outb(CrtControllerIndex, 0x11);
    uint8_t x = inb(CrtControllerData);
    outb(CrtControllerData, x & 0x7f);

    outw(CrtControllerIndex, 0x0d06);
    outw(CrtControllerIndex, 0x3e07);
    outw(CrtControllerIndex, 0x4109);
    outw(CrtControllerIndex, 0xea10);
    outw(CrtControllerIndex, 0xac11);
    outw(CrtControllerIndex, 0xdf12);
    outw(CrtControllerIndex, 0x0014);
    outw(CrtControllerIndex, 0xe715);
    outw(CrtControllerIndex, 0x0616);
    outw(CrtControllerIndex, 0xe317);

    outw(SequencerIndex, 0x0f02);
    outw(SequencerIndex, 0x0300);
#endif

    SetupPallette();
}

void DrawTestPattern()
{
#if 1
    // ModeX
    uint32_t* mem = (uint32_t*)GetVramBaseAddr();
    for (uint8_t y = 0; y < 1; y++)
    {
        for (uint16_t x = 0; x < 80; x++)
        {
            mem[y * 80 + x] = 0xFFffFFff;
        }
    }
#else
    // Mode 13H
    uint8_t* mem = (uint8_t*)VramBase;
    for (uint8_t y = 0; y < 100; y++)
    {
        for (uint16_t x = 0; x < 320; x++)
        {
            mem[y * 320 + x] = 0xFF;
        }
    }
#endif
}
}