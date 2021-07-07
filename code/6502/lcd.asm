!cpu 65c02
!initmem $EF
!to "lcd.o", plain

*=$8000

LCD_CMD  = $7f00
LCD_DATA = $7f01

LCD_CMD_CLEAR                = %00000001
LCD_CMD_HOME                 = %00000010

LCD_CMD_ENTRY_MODE           = %00000100
LCD_CMD_ENTRY_MODE_INCREMENT = %00000010
LCD_CMD_ENTRY_MODE_DECREMENT = %00000000
LCD_CMD_ENTRY_MODE_SHIFT     = %00000001

LCD_CMD_DISPLAY              = %00001000
LCD_CMD_DISPLAY_ON           = %00000100
LCD_CMD_DISPLAY_CURSOR       = %00000010
LCD_CMD_DISPLAY_CURSOR_BLINK = %00000001

LCD_CMD_SHIFT                = %00010000
LCD_CMD_SHIFT_CURSOR         = %00000000
LCD_CMD_SHIFT_DISPLAY        = %00001000
LCD_CMD_SHIFT_LEFT           = %00000000
LCD_CMD_SHIFT_RIGHT          = %00000100

LCD_CMD_SET_CGRAM_ADDR       = %01000000
LCD_CMD_SET_DRAM_ADDR        = %10000000

LCD_CMD_FUNCTIONSET = $20
LCD_CMD_8BITMODE    = $10
LCD_CMD_2LINE       = $08

LCD_INITIALIZE      = <(LCD_CMD_FUNCTIONSET | LCD_CMD_8BITMODE | LCD_CMD_2LINE)
DISPLAY_MODE  = <(LCD_CMD_DISPLAY | LCD_CMD_DISPLAY_ON | LCD_CMD_DISPLAY_CURSOR | LCD_CMD_DISPLAY_CURSOR_BLINK)


lda #LCD_INITIALIZE
sta LCD_CMD
lda #DISPLAY_MODE
sta LCD_CMD
lda #LCD_CMD_CLEAR
sta LCD_CMD

outText:
lda #'H'
sta LCD_DATA
lda #'e'
sta LCD_DATA
lda #'l'
sta LCD_DATA
lda #'l'
sta LCD_DATA
lda #'o'
sta LCD_DATA
lda #','
sta LCD_DATA
lda #' '
sta LCD_DATA
lda #'W'
sta LCD_DATA
lda #'o'
sta LCD_DATA
lda #'r'
sta LCD_DATA
lda #'l'
sta LCD_DATA
lda #'d'
sta LCD_DATA
lda #'!'
sta LCD_DATA
jmp outText


*=$FFFC
!word $8000