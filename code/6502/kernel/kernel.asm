; 6502 - HBC-56 Kernel
;
; Copyright (c) 2021 Troy Schrapel
;
; This code is licensed under the MIT license
;
; https://github.com/visrealm/hbc-56
;
;

HBC56_INT_VECTOR = $7e00
HBC56_NMI_VECTOR = $7e04
HBC56_RST_VECTOR = kernelMain

HBC56_KERNEL_START = $e800
HBC56_META_VECTOR  = HBC56_KERNEL_START-4

TMS_MODEL = 9918
RTI_OPCODE = $40
JMP_OPCODE = $4c

LCD_MODEL = 12864
!ifndef LCD_MODEL {
        LCD_MODEL = 12864
}
!src "hbc56.asm"
*=HBC56_KERNEL_START

+hbc56Title "github.com/visrealm/hbc-56"

!src "ut/bcd.asm"
!src "ut/memory.asm"


!src "inp/nes.asm"
!src "inp/keyboard.asm"

!src "gfx/tms9918.asm"
!src "sfx/ay3891x.asm"
!src "gfx/bitmap.asm"
!src "lcd/lcd.asm"

!src "kernel_macros.asm"

!src "bootscreen.asm"

HBC56_TICKS         = $7e80
HBC56_SECONDS_L     = HBC56_TICKS + 1
HBC56_SECONDS_H     = HBC56_SECONDS_L + 1
HBC56_TMP           = HBC56_SECONDS_H + 1

HBC56_CONSOLE_FLAGS = HBC56_TMP + 1
HBC56_CONSOLE_FLAG_CURSOR = $80
HBC56_CONSOLE_FLAG_NES    = $40
HBC56_CONSOLE_FLAG_LCD    = $20

HBC56_TMP_X     = HBC56_CONSOLE_FLAGS + 1
HBC56_TMP_Y     = HBC56_CONSOLE_FLAGS + 2

onVSync:
        pha
        
        ; this is just for the emulator... 
        ; otherwise, we only see this interrupt handler repeating in the debuggeer
        +tmsDisableInterrupts 

        inc HBC56_TICKS
        lda HBC56_TICKS
        cmp #TMS_FPS
        bne +
        lda #0
        sta HBC56_TICKS
        +inc16 HBC56_SECONDS_L
+
        bit HBC56_CONSOLE_FLAGS
        bpl ++
        stx HBC56_TMP_X
        sty HBC56_TMP_Y
        jsr tmsSetPosConsole
        ldx HBC56_TMP_X
        ldy HBC56_TMP_Y
        lda HBC56_TICKS
        cmp #30
        bcc +
        lda #' '
        +tmsPut
        jmp ++
+ 
        lda #$7f
        +tmsPut
++
        +tmsEnableInterrupts 

        +tmsReadStatus
        pla      
        rti

consoleEnableCursor:
        +consoleEnableCursor
        rts

consoleDisableCursor:
        +consoleDisableCursor
        rts

hbc56Bell:
        +ayPlayNote AY_PSG0, AY_CHC, NOTE_C
        jsr hbc56Delay
        jsr hbc56Delay
        +ayStop AY_PSG0, AY_CHC
        rts


kernelMain:
        cld     ; make sure we're not in decimal mode
        ldx #$ff
        txs

        lda #RTI_OPCODE
        sta HBC56_INT_VECTOR
        sta HBC56_NMI_VECTOR

        lda #0
        sta HBC56_TICKS
        sta HBC56_SECONDS_L
        sta HBC56_SECONDS_H
        sta HBC56_CONSOLE_FLAGS

        sei
        jsr kbInit
        jsr ayInit

        +ayWrite AY_PSG0, AY_ENABLES, $3b
        +ayWrite AY_PSG0, AY_CHC_AMPL, $0f
        +ayWrite AY_PSG0, AY_ENV_PERIOD_L, $00
        +ayWrite AY_PSG0, AY_ENV_PERIOD_H, $80;.duration
        +ayWrite AY_PSG0, AY_ENV_SHAPE, $0e

        !ifdef tmsInit { jsr tmsInit }
        !ifdef lcdInit { jsr lcdInit }

        jsr hbc56BootScreen

        jsr HBC56_META_VECTOR   ; user program metadata

        +tmsEnableOutput

        lda #20
        sta HBC56_TMP
-
        jsr hbc56Delay
        dec HBC56_TMP
        bne -

        lda HBC56_CONSOLE_FLAGS
        and #HBC56_CONSOLE_FLAG_LCD
        bne .afterInput         ; LCD - skip input

        lda #HBC56_CONSOLE_FLAG_NES
        and HBC56_CONSOLE_FLAGS
        beq .keyboardInput


        ; NES input
        +tmsPrintZ HBC56_PRESS_ANY_NES_TEXT, (32 - HBC56_PRESS_ANY_NES_TEXT_LEN) / 2, 15
        jsr nesWaitForPress
        jmp .afterInput

.keyboardInput
        ; Keyboard  input
        +tmsPrintZ HBC56_PRESS_ANY_KEY_TEXT, (32 - HBC56_PRESS_ANY_KEY_TEXT_LEN) / 2, 15
        jsr kbWaitForKey

.afterInput

        jsr hbc56Delay

        cli

        +tmsDisableOutput

        +setIntHandler onVSync

        jmp DEFAULT_HBC56_RST_VECTOR

;!warn "Kernel size: ", *-$f000
;!warn "Bytes remaining: ", DEFAULT_HBC56_INT_VECTOR-*