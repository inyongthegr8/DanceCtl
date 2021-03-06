/*
 * Copyright (c) 2020 Martin Natano <natano@natano.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * 6-14-20 
 * Minor edits made by Todd Oakes, under the above license agreement
 */

#include "HID.h"

static const uint8_t hidReportDescriptor[] PROGMEM = {
    0x05, 0x01,   // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,   // USAGE (Game Pad)
    0xa1, 0x01,   // COLLECTION (Application)
    0xa1, 0x00,   //  COLLECTION (Physical)
    0x85, 0x03,   //    REPORT_ID (3)

    // Buttons
    0x05, 0x09,   //    USAGE_PAGE (Button)
    0x19, 0x01,   //    USAGE_MINIMUM (Button 1)
    0x29, 0x08,   //    USAGE_MAXIMUM (Button 8)
    0x15, 0x00,   //    LOGICAL_MINIMUM (0)
    0x25, 0x01,   //    LOGICAL_MAXIMUM (1)
    0x75, 0x01,   //    REPORT_SIZE (1)
    0x95, 0x08,   //    REPORT_COUNT (8)
    0x81, 0x02,   //    INPUT (Data, Var, Abs)

    // Padding
    0x75, 0x01,   //    REPORT_SIZE (1)
    0x95, 0x02,   //    REPORT_COUNT (2)
    0x01, 0x01,   //    INPUT (Constant)

    0xc0,         //  END_COLLECTION
    0xc0,         // END_COLLECTION
};

#define DEBOUNCE_DELAY 2
#define NBUTTONS 8
static const int buttonPins[NBUTTONS] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
    int i;
    
    pinMode(LED_BUILTIN, OUTPUT);
    for (i = 0; i < NBUTTONS; i++)
        pinMode(buttonPins[i], INPUT_PULLUP);

    static HIDSubDescriptor node(hidReportDescriptor, sizeof(hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

void loop() {
    static uint8_t reportData;
    static uint8_t buttonStates;
    static unsigned long lastButtonChange[NBUTTONS];
    unsigned long now;
    int sendReport, i;

    now = millis();
    sendReport = 0;

    for (i = 0; i < NBUTTONS; i++) {
        int pressed = !digitalRead(buttonPins[i]);
        if (pressed != bitRead(buttonStates, i)) {
            bitWrite(buttonStates, i, pressed);
            lastButtonChange[i] = now;
        }

        if (pressed != bitRead(reportData, i) && lastButtonChange[i] + DEBOUNCE_DELAY < now) {
            bitWrite(reportData, i, pressed);
            sendReport = 1;
        }
    }

    if (sendReport) {
        HID().SendReport(0x03, &reportData, sizeof(reportData));
        digitalWrite(LED_BUILTIN, reportData ? HIGH : LOW);
    }
}
