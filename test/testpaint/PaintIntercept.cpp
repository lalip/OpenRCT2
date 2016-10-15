#pragma region Copyright (c) 2014-2016 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion


#include "intercept.h"

extern "C" {
#include "../../src/common.h"
#include "../../src/hook.h"
#include "../../src/interface/viewport.h"
}

class PaintInterceptor {
public:
    static void InitHooks() {
        addhook(0x006629BC, (int) InterceptWoodenASupports, 0, (int[]) {EAX, EBX, EDX, EDI, EBP, END}, 0, EAX);
        addhook(0x00662D5C, (int) InterceptWoodenBSupports, 0, (int[]) {EAX, EBX, EDX, EDI, EBP, END}, 0, EAX);

        addhook(0x00663105, (int) InterceptMetalASupports, 0, (int[]) {EAX, EBX, EDX, EDI, EBP, END}, 0, EAX);
        addhook(0x00663584, (int) InterceptMetalBSupports, 0, (int[]) {EAX, EBX, EDX, EDI, EBP, END}, 0, EAX);

        addhook(0x006861AC, (int) InterceptPaint6C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x00686337, (int) InterceptPaint6C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x006864D0, (int) InterceptPaint6C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x0068666B, (int) InterceptPaint6C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);

        addhook(0x00686806, (int) InterceptPaint7C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x006869B2, (int) InterceptPaint7C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x00686B6F, (int) InterceptPaint7C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x00686D31, (int) InterceptPaint7C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);

        addhook(0x00686EF0, (int) InterceptPaint8C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x00687056, (int) InterceptPaint8C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x006871C8, (int) InterceptPaint8C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x0068733C, (int) InterceptPaint8C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);

        addhook(0x006874B0, (int) InterceptPaint9C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x00687618, (int) InterceptPaint9C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x0068778C, (int) InterceptPaint9C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
        addhook(0x00687902, (int) InterceptPaint9C, 0, (int[]) {EAX, EBX, ECX, EDX, ESI, EDI, EBP, END}, 0, EBP);
    }

    static bool PaintWoodenSupports(uint8 function, int supportType, int special, int height, uint32 imageColourFlags, bool *underground) {
        function_call call = {
            .function = function,
            .supports = {
                .type = supportType,
                .special = special,
                .height = height,
                .colour_flags = imageColourFlags,
            }
        };

        _calls[_callCount] = call;
        _callCount++;

        return _woodenSupports;
    }

    static bool PaintMetalSupports(uint8 function, int supportType, uint8 segment, int special, int height, uint32 imageColourFlags) {
        CheckSegmentSupportHeight();

        function_call call = {
            .function = function,
            .supports = {
                .type = supportType,
                .segment = segment,
                .special = special,
                .height = height,
                .colour_flags = imageColourFlags,
            }
        };

        _calls[_callCount] = call;
        _callCount++;
        return false;
    }

    static paint_struct *Paint6C(
        uint32 imageID,
        sint8 xOffset, sint8 yOffset,
        sint16 boundBoxLengthX, sint16 boundBoxLengthY, sint8 boundBoxLengthZ,
        sint16 zOffset,
        uint32 rotation
    ) {
        function_call call = {
            .function = PAINT_98196C,
            .paint = {
                .image_id = imageID,
                .offset = {xOffset, yOffset},
                .bound_box_length = {boundBoxLengthX, boundBoxLengthY, boundBoxLengthZ},
                .z_offset = zOffset,
                .rotation = rotation
            },
        };

        _calls[_callCount] = call;
        _callCount++;

        return nullptr;
    }

    static paint_struct *PaintFull(
        uint8 function,
        uint32 imageID,
        sint8 xOffset, sint8 yOffset,
        sint16 boundBoxLengthX, sint16 boundBoxLengthY, sint8 boundBoxLengthZ,
        sint16 zOffset,
        sint16 boundBoxOffsetX, sint16 boundBoxOffsetY, sint16 boundBoxOffsetZ,
        uint32 rotation
    ) {
        function_call call = {
            .function = function,
            .paint = {
                .image_id = imageID,
                .offset = {xOffset, yOffset},
                .bound_box_length = {boundBoxLengthX, boundBoxLengthY, boundBoxLengthZ},
                .bound_box_offset = {boundBoxOffsetX, boundBoxOffsetY, boundBoxOffsetZ},
                .z_offset = zOffset,
                .rotation = rotation
            },
        };

        _calls[_callCount] = call;
        _callCount++;

        return nullptr;
    }

    static void ClearCalls() {
        _callCount = 0;
        memset(_calls, 0, sizeof(_calls));
    }

    static int GetCalls(function_call *buffer) {
        memcpy(buffer, _calls, _callCount * sizeof(function_call));
        return _callCount;
    }

    static void SetSimulateWoodenSupports(bool enabled) {
        _woodenSupports = enabled;
    }

private:
    static bool _woodenSupports;
    static uint8 _callCount;
    static function_call _calls[256];

    static uint32 InterceptMetalASupports(uint32 eax, uint32 ebx, uint32 edx, uint32 edi, uint32 ebp) {
        bool output = PaintMetalSupports(SUPPORTS_METAL_A, edi, ebx, (sint16) (eax & 0xFFFF), (edx & 0xFFFF), ebp);

        return output ? 1 : 0;
    }

    static uint32 InterceptMetalBSupports(uint32 eax, uint32 ebx, uint32 edx, uint32 edi, uint32 ebp) {
        bool output = PaintMetalSupports(SUPPORTS_METAL_B, edi, ebx, (sint16) (eax & 0xFFFF), (edx & 0xFFFF), ebp);

        return output ? 1 : 0;
    }

    static void CheckSegmentSupportHeight() {
        // First get last known support height state
        if (memcmp(gSupportSegments, &DefaultSegmentHeight, sizeof(support_height) * 9) == 0) {
            // Nothing changed
            return;
        }


        function_call call = {
            .function = SET_SEGMENT_HEIGHT
        };

        _calls[_callCount] = call;
        _callCount++;
    }

    static uint32 InterceptWoodenASupports(uint32 eax, uint32 ebx, uint32 edx, uint32 edi, uint32 ebp) {
        bool output = PaintWoodenSupports(SUPPORTS_WOOD_A, edi, eax & 0xFFFF, edx & 0xFFFF, ebp, nullptr);

        return output ? 1 : 0;
    }

    static uint32 InterceptWoodenBSupports(uint32 eax, uint32 ebx, uint32 edx, uint32 edi, uint32 ebp) {
        bool output = PaintWoodenSupports(SUPPORTS_WOOD_B, edi, eax & 0xFFFF, edx & 0xFFFF, ebp, nullptr);

        return output ? 1 : 0;
    }

    static uint32 InterceptPaint6C(uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esi, uint32 edi, uint32 ebp) {
        if ((ebp & 0x03) != get_current_rotation()) {
            // Log error
            log_error("Ebp is different from current rotation");
        }

        return (uintptr_t) Paint6C(
            ebx,
            (sint8) (eax & 0xFF), (sint8) (ecx & 0xFF),
            (sint16) (edi & 0xFFFF), (sint16) (esi & 0xFFFF), (sint8) ((eax >> 8) & 0xFF),
            edx & 0xFFFF,
            ebp & 0x03
        );
    }

    static uint32 InterceptPaint7C(uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esi, uint32 edi, uint32 ebp) {
        return InterceptPaintFull(PAINT_98197C, eax, ebx, ecx, edx, esi, edi, ebp);
    }

    static uint32 InterceptPaint8C(uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esi, uint32 edi, uint32 ebp) {
        return InterceptPaintFull(PAINT_98198C, eax, ebx, ecx, edx, esi, edi, ebp);
    }

    static uint32 InterceptPaint9C(uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esi, uint32 edi, uint32 ebp) {
        return InterceptPaintFull(PAINT_98199C, eax, ebx, ecx, edx, esi, edi, ebp);
    }

    static uint32 InterceptPaintFull(uint8 function, uint32 eax, uint32 ebx, uint32 ecx, uint32 edx, uint32 esi, uint32 edi, uint32 ebp) {
        if ((ebp & 0x03) != get_current_rotation()) {
            // Log error
            log_error("Ebp is different from current rotation");
        }

        rct_xyz16 boundOffset = {
            RCT2_GLOBAL(RCT2_ADDRESS_PAINT_BOUNDBOX_OFFSET_X, sint16),
            RCT2_GLOBAL(RCT2_ADDRESS_PAINT_BOUNDBOX_OFFSET_Y, sint16),
            RCT2_GLOBAL(RCT2_ADDRESS_PAINT_BOUNDBOX_OFFSET_Z, sint16)
        };

        return (uintptr_t) PaintFull(
            function,
            ebx,
            (sint8) (eax & 0xFF), (sint8) (ecx & 0xFF),
            (sint16) (edi & 0xFFFF), (sint16) (esi & 0xFFFF), (sint8) ((eax >> 8) & 0xFF),
            edx & 0xFFFF,
            boundOffset.x, boundOffset.y, boundOffset.z,
            ebp & 0x03
        );
    }
};

bool PaintInterceptor::_woodenSupports = false;
uint8 PaintInterceptor::_callCount = 0;
function_call PaintInterceptor::_calls[256] = {0};

extern "C" {
bool wooden_a_supports_paint_setup(int supportType, int special, int height, uint32 imageColourFlags, bool *underground) {
    return PaintInterceptor::PaintWoodenSupports(SUPPORTS_WOOD_A, supportType, special, height, imageColourFlags, underground);
}

bool wooden_b_supports_paint_setup(int supportType, int special, int height, uint32 imageColourFlags, bool *underground) {
    return PaintInterceptor::PaintWoodenSupports(SUPPORTS_WOOD_B, supportType, special, height, imageColourFlags, underground);
}

bool metal_a_supports_paint_setup(uint8 supportType, uint8 segment, int special, int height, uint32 imageColourFlags) {
    return PaintInterceptor::PaintMetalSupports(SUPPORTS_METAL_A, supportType, segment, special, height, imageColourFlags);
}

bool metal_b_supports_paint_setup(uint8 supportType, uint8 segment, int special, int height, uint32 imageColourFlags) {
    return PaintInterceptor::PaintMetalSupports(SUPPORTS_METAL_B, supportType, segment, special, height, imageColourFlags);
}

paint_struct *sub_98196C(uint32 image_id, sint8 x_offset, sint8 y_offset, sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z, sint16 z_offset, uint32 rotation) {
    return PaintInterceptor::Paint6C(image_id, x_offset, y_offset, bound_box_length_x, bound_box_length_y, bound_box_length_z, z_offset, rotation);
}

paint_struct *sub_98197C(
    uint32 image_id,
    sint8 x_offset, sint8 y_offset,
    sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z,
    sint16 z_offset,
    sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z,
    uint32 rotation
) {
    return PaintInterceptor::PaintFull(
        PAINT_98197C,
        image_id,
        x_offset, y_offset,
        bound_box_length_x, bound_box_length_y, bound_box_length_z,
        z_offset,
        bound_box_offset_x, bound_box_offset_y, bound_box_offset_z,
        rotation
    );
}

paint_struct *sub_98198C(
    uint32 image_id,
    sint8 x_offset, sint8 y_offset,
    sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z,
    sint16 z_offset,
    sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z,
    uint32 rotation
) {
    return PaintInterceptor::PaintFull(
        PAINT_98198C,
        image_id,
        x_offset, y_offset,
        bound_box_length_x, bound_box_length_y, bound_box_length_z,
        z_offset,
        bound_box_offset_x, bound_box_offset_y, bound_box_offset_z,
        rotation
    );
}

paint_struct *sub_98199C(
    uint32 image_id,
    sint8 x_offset, sint8 y_offset,
    sint16 bound_box_length_x, sint16 bound_box_length_y, sint8 bound_box_length_z,
    sint16 z_offset,
    sint16 bound_box_offset_x, sint16 bound_box_offset_y, sint16 bound_box_offset_z,
    uint32 rotation
) {
    return PaintInterceptor::PaintFull(
        PAINT_98199C,
        image_id,
        x_offset, y_offset,
        bound_box_length_x, bound_box_length_y, bound_box_length_z,
        z_offset,
        bound_box_offset_x, bound_box_offset_y, bound_box_offset_z,
        rotation
    );
}

bool paint_attach_to_previous_ps(uint32 image_id, uint16 x, uint16 y) {
    return false;
}

void intercept_clear_calls() {
    PaintInterceptor::ClearCalls();
}

int intercept_get_calls(function_call *buffer) {
    return PaintInterceptor::GetCalls(buffer);
}

void intercept_simulate_wooden_supports(bool enabled) {
    return PaintInterceptor::SetSimulateWoodenSupports(enabled);
}

void initHooks() {
    PaintInterceptor::InitHooks();
}

}
