#include "MemoryProtectiveUnit.h"

static void DisableMemoryProtectiveUnit(void)
{
    /*
    
    appnotes/files/apnt209.pdf
    
    System Handler Control and State Register SHCSR
    
    Bit 16, MEMFAULTENA: Memory Management Fault exception enable bit, set to 1 to enable; set to 0 to disable
    
    */
    SCB -> SHCSR &= ~(0x01 << 16);
    
    /*
    
    MPU Region Attribute and Size Register Value.
    
    */
    
    /*
    
    Bits	Name	Function
    [31:3]	-	Reserved.
    [2]	PRIVDEFENA	0 - Disables the default memory map. 1 - Enables the default memory map as a background region for privileged access.
    [1]	HFNMIENA	0 - Disables the MPU for exception handlers. 1 - Use the MPU for memory accesses by exception handlers.
    [0]	ENABLE	0 - The MPU is disabled. 1 - The MPU is enabled.
    
    
    MPU_Type::RASR
    Offset: 0x010 (R/W) MPU Region Attribute and Size Register.

    Defines the size and access behavior of the region identified by MPU_RNR, and enables that region.

    Bits	Name	Function
    [31:29]	-	Reserved.
    [28]	XN	Execute Never.
    [27]	-	Reserved.
    [26:24]	AP	Access Permissions, see ARM_MPU_AP_xxx.
    [23:22]	-	Reserved.
    [21:19]	TEX	Type Extension.
    [18]	S	Shareable.
    [17]	C	Cacheable.
    [16]	B	Bufferable.
    [15:8]	SRD	Subregion Disable. For regions of 256 bytes or larger, each bit of this field controls whether one of the eight equal subregions is enabled (0) or disabled (1).
    [7:6]	-	Reserved.
    [5:1]	SIZE	Indicates the region size. The region size, in bytes, is 2(SIZE+1). SIZE field values less than 4 are reserved, because the smallest supported region size is 32 bytes.
    [0]	ENABLE	0 - This region is disabled. 1 - This region is enabled.

    */
    MPU -> CTRL &= ~(0x01 << 0);
}

static void EnableMemoryProtectiveUnit(void)
{
    /*
    
    Bits	Name	Function
    [31:3]	-	Reserved.
    [2]	PRIVDEFENA	0 - Disables the default memory map. 1 - Enables the default memory map as a background region for privileged access.
    [1]	HFNMIENA	0 - Disables the MPU for exception handlers. 1 - Use the MPU for memory accesses by exception handlers.
    [0]	ENABLE	0 - The MPU is disabled. 1 - The MPU is enabled.
    
    */
    MPU -> CTRL |= 0x01 << 2;
    MPU -> CTRL |= 0x01 << 0;
    
    SCB -> SHCSR |= 0x01 << 16;
}

static uint8_t ComputeExponent(uint32_t bytes)
{
    uint8_t exponent = 0;
    
    while (!bytes)
    {
        bytes >>= 1;
        
        exponent ++;
    }
    
    return exponent;
}

uint8_t SetMemoryProtectiveRegion(uint32_t baseAddress, uint32_t size, uint8_t regionNunber, uint8_t instructiveEnabled, uint8_t accessPrivilege, uint8_t shareEnabled, uint8_t cacheEnabled, uint8_t bufferEnabled)
{    
    if ((size % 32) || size == 0)
        return 1;
    
    DisableMemoryProtectiveUnit();
    
    /*
    
    MPU Region Base Address Register Value.

    This preprocessor function can be used to construct a valid RBAR value. The VALID bit is implicitly set to 1.
        
    */
    MPU -> RBAR = baseAddress;
    
    /*
    
    Region Number Register
    
    */
    MPU -> RNR = regionNunber;
    
    uint32_t bits = 0;
    
    /*
    
    MPU Region Attribute and Size Register, MPU_RASR, Defines the size, access behavior, and memory type of the region identified by MPU_RNR, and enables that region.
    
    https://developer.arm.com/documentation/ddi0419/c/System-Level-Architecture/System-Address-Map/Protected-Memory-System-Architecture--PMSAv6/MPU-Region-Attribute-and-Size-Register--MPU-RASR?lang=en
    
    Bits	Name	Function
    
    [31:16]	ATTRS	
    The MPU Region Attribute field, This field has the following subfields, defined in Region attribute control:

    [7:6]	-	Reserved

    */
    
    /*
    
    XN
    MPU_RASR[28]
    
    */
    bits |= instructiveEnabled << 28;
    
    /*
    
    AP[2:0]
    MPU_RASR[26:24]
    
    */
    bits |= accessPrivilege << 24;
    
    /*
    
    TEX[2:0]
    MPU_RASR[21:19]
    
    */
    
    bits |= 0x00 << 19;
    
    /*
    
    S
    MPU_RASR[18]
    
    */
    bits |= shareEnabled << 18;
    
    /*
    
    C
    MPU_RASR[17]
    
    */
    
    bits |= cacheEnabled << 17;
    
    /*
    B
    MPU_RASR[16]
    
    */
    
    bits |= bufferEnabled << 16;
    
    /*
    
    [15:8]	SRD	
    Subregion Disable. For regions of 256 bytes or larger, each bit of this field controls whether one of the eight equal subregions is enabled, see Memory region subregions:

    0
    subregion enabled.

    1
    subregion disabled.
    
    */
    bits |= 0x00 << 8;
    
    /*
    [5:1]	SIZE	
    Indicates the region size.

    The permitted values for SIZE are 7-31, that is 0b00111-0b11111. The associated region size, in bytes, is 2(SIZE+1). SIZE field values less than 7 are reserved, because the smallest supported region size is 256 bytes.

    */
    
    uint8_t exponent = ComputeExponent(size) - 1;
    
    bits |= exponent << 1;
    
    /*
    
    [0]	ENABLE	
    Enables this region:

    0
    when the MPU is enabled, this region is disabled.

    1
    When the MPU is enabled, this region is enabled.

    Enabling a region has no effect unless the MPU_CTRL.ENABLE bit is set to 1, to enable the MPU.
    
    */
    
    bits |= 0x01 << 0;
    
    MPU -> RASR = bits;
    
    EnableMemoryProtectiveUnit();
    
    return 0;
}

void SetProtectiveRegions(void)
{
    /* DTCM. */
    SetMemoryProtectiveRegion(0x20000000, 128 * 1024, 1, 0, MPU_REGION_FULL_ACCESS, 0, 1, 1);
    
    /* AXI SRAM. */
    SetMemoryProtectiveRegion(0x24000000, 512 * 1024, 2, 0, MPU_REGION_FULL_ACCESS, 0, 1, 1);
    
    /* SRAM1 - SRAM3. */
    SetMemoryProtectiveRegion(0x30000000, 288 * 1024, 3, 0, MPU_REGION_FULL_ACCESS, 0, 1, 1);
    
    /* SRAM4. */
    SetMemoryProtectiveRegion(0x38000000, 64 * 1024, 4, 0, MPU_REGION_FULL_ACCESS, 0, 1, 1);
    
    /* SDRAM. */
    SetMemoryProtectiveRegion(0xC0000000, 64 * 1024 * 1024, 5, 0, MPU_REGION_FULL_ACCESS, 0, 1, 1);
    
    /* NAND. */
    SetMemoryProtectiveRegion(0x80000000, 256 * 1024 * 1024, 6, 1, MPU_REGION_FULL_ACCESS, 0, 0, 0);
}