/**************************************************************************************
 * include/nuttx/lcd/ssd1306.h
 *
 * Driver for Univision UG-2864HSWEG01 OLED display or UG-2832HSWEG04 both with the
 * Univision SSD1306 controller in SPI mode and Densitron DD-12864WO-4A with SSD1309
 * in SPI mode.
 *
 *   Copyright (C) 2012-2013, 2015, 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * References:
 *   1. Product Specification (Preliminary), Part Name: OEL Display Module, Part ID:
 *      UG-2864HSWEG01, Doc No: SAS1-9046-B, Univision Technology Inc.
 *   2. Product Specification, Part Name: OEL Display Module, Part ID: UG-2832HSWEG04,
 *      Doc No.: SAS1-B020-B, Univision Technology Inc.
 *   3. SSD1306, 128 X 64 Dot Matrix OLED/PLED, Preliminary Segment/Common Driver with
 *      Controller,  Solomon Systech
 *   4. SSD1309, 128 x 64 Dot Matrix OLED/PLED Segment/Common Driver with Controller,
 *      Solomon Systech
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************/

#ifndef __INCLUDE_NUTTX_SSD1306_H
#define __INCLUDE_NUTTX_SSD1306_H

/**************************************************************************************
 * Included Files
 **************************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>

#include <nuttx/arch.h>

#ifdef CONFIG_LCD_SSD1306

/**************************************************************************************
 * Pre-processor Definitions
 **************************************************************************************/
/* Configuration **********************************************************************/
/* UG-2864HSWEG01 Configuration Settings:
 *
 * CONFIG_UG2864HSWEG01_SPIMODE - Controls the SPI mode
 * CONFIG_UG2864HSWEG01_FREQUENCY - Define to use a different bus frequency
 * CONFIG_UG2864HSWEG01_NINTERFACES - Specifies the number of physical UG-2864HSWEG01
 *   devices that will be supported.
 *
 * Required LCD driver settings:
 *
 * CONFIG_LCD_UG28HSWEG01 - Enable UG-2864HSWEG01 support
 * CONFIG_LCD_MAXCONTRAST should be 255, but any value >0 and <=255 will be accepted.
 * CONFIG_LCD_MAXPOWER must be 1
 *
 * Option LCD driver settings:
 * CONFIG_LCD_LANDSCAPE, CONFIG_LCD_PORTRAIT, CONFIG_LCD_RLANDSCAPE, and
 *   CONFIG_LCD_RPORTRAIT - Display orientation.
 *
 * Required SPI driver settings:
 * CONFIG_SPI_CMDDATA - Include support for cmd/data selection.
 */

/* SPI Interface
 *
 * "The serial interface consists of serial clock SCL, serial data SI, A0 and
 *  CS . SI is shifted into an 8-bit shift register on every rising edge of
 *  SCL in the order of D7, D6, � and D0. A0 is sampled on every eighth clock
 *  and the data byte in the shift register is written to the display data RAM
 *  or command register in the same clock."
 *
 * MODE 3:
 *   Clock polarity:  High (CPOL=1)
 *   Clock phase:     Sample on trailing (rising edge) (CPHA 1)
 */

#ifdef LCD_SSD1306_SPI

#ifndef CONFIG_UG2864HSWEG01_SPIMODE
#  define CONFIG_UG2864HSWEG01_SPIMODE SPIDEV_MODE3
#endif

/* "This module determines whether the input data is interpreted as data or
 * command. When A0 = �H�, the inputs at D7 - D0 are interpreted as data and be
 * written to display RAM. When A0 = �L�, the inputs at D7 - D0 are interpreted
 * as command, they will be decoded and be written to the corresponding command
 * registers.
 */

#ifndef CONFIG_SPI_CMDDATA
#  error "CONFIG_SPI_CMDDATA must be defined in your NuttX configuration"
#endif

#endif /* CONFIG_LCD_SSD1306_SPI */

#ifdef CONFIG_LCD_SSD1306_I2C

#ifndef CONFIG_SSD1306_I2CADDR
#  define CONFIG_SSD1306_I2CADDR 0x78 /* 120 in decimal */
#endif

#ifndef CONFIG_SSD1306_I2CFREQ
#  define CONFIG_SSD1306_I2CADDR 400000
#endif

#endif /* CONFIG_LCD_SSD1306_I2C */

/* CONFIG_UG2864HSWEG01_NINTERFACES determines the number of physical interfaces
 * that will be supported.
 */

#ifndef CONFIG_UG2864HSWEG01_NINTERFACES
#  define CONFIG_UG2864HSWEG01_NINTERFACES 1
#endif

/* Check contrast selection */

#if !defined(CONFIG_LCD_MAXCONTRAST)
#  define CONFIG_LCD_MAXCONTRAST 255
#endif

#if CONFIG_LCD_MAXCONTRAST <= 0|| CONFIG_LCD_MAXCONTRAST > 255
#  error "CONFIG_LCD_MAXCONTRAST exceeds supported maximum"
#endif

#if CONFIG_LCD_MAXCONTRAST < 255
#  warning "Optimal setting of CONFIG_LCD_MAXCONTRAST is 255"
#endif

/* Check power setting */

#if !defined(CONFIG_LCD_MAXPOWER)
#  define CONFIG_LCD_MAXPOWER 1
#endif

#if CONFIG_LCD_MAXPOWER != 1
#  warning "CONFIG_LCD_MAXPOWER exceeds supported maximum"
#  undef CONFIG_LCD_MAXPOWER
#  define CONFIG_LCD_MAXPOWER 1
#endif

/* Color is 1bpp monochrome with leftmost column contained in bits 0  */

#ifdef CONFIG_NX_DISABLE_1BPP
#  warning "1 bit-per-pixel support needed"
#endif

/* Orientation */

#if defined(CONFIG_LCD_LANDSCAPE)
#  undef CONFIG_LCD_PORTRAIT
#  undef CONFIG_LCD_RLANDSCAPE
#  undef CONFIG_LCD_RPORTRAIT
#elif defined(CONFIG_LCD_PORTRAIT)
#  undef CONFIG_LCD_LANDSCAPE
#  undef CONFIG_LCD_RLANDSCAPE
#  undef CONFIG_LCD_RPORTRAIT
#elif defined(CONFIG_LCD_RLANDSCAPE)
#  undef CONFIG_LCD_LANDSCAPE
#  undef CONFIG_LCD_PORTRAIT
#  undef CONFIG_LCD_RPORTRAIT
#elif defined(CONFIG_LCD_RPORTRAIT)
#  undef CONFIG_LCD_LANDSCAPE
#  undef CONFIG_LCD_PORTRAIT
#  undef CONFIG_LCD_RLANDSCAPE
#else
#  define CONFIG_LCD_LANDSCAPE 1
#  warning "Assuming landscape orientation"
#endif

/* Some important "colors" */

#define SSD1306_Y1_BLACK  0
#define SSD1306_Y1_WHITE  1

/**************************************************************************************
 * Public Types
 **************************************************************************************/

struct ssd1306_priv_s
{
  bool (*set_vcc) (bool on); /* Allow board to control display power. Return true if
                                request state set successfully. */
};

/**************************************************************************************
 * Public Data
 **************************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************************
 * Public Function Prototypes
 **************************************************************************************/

/**************************************************************************************
 * Name:  ssd1306initialize
 *
 * Description:
 *   Initialize the UG-2864HSWEG01 video hardware.  The initial state of the
 *   OLED is fully initialized, display memory cleared, and the OLED ready
 *   to use, but with the power setting at 0 (full off == sleep mode).
 *
 * Input Parameters:
 *
 *   dev - A reference to the SPI/I2C driver instance.
 *   board_priv - Board specific structure.
 *   devno - A value in the range of 0 through CONFIG_UG2864HSWEG01_NINTERFACES-1.
 *     This allows support for multiple OLED devices.
 *
 * Returned Value:
 *
 *   On success, this function returns a reference to the LCD object for
 *   the specified OLED.  NULL is returned on any failure.
 *
 **************************************************************************************/

struct lcd_dev_s;    /* See include/nuttx/lcd/lcd.h */
struct spi_dev_s;    /* See include/nuttx/spi/spi.h */
struct i2c_master_s; /* See include/nuttx/i2c/i2c_master.h */

#ifdef CONFIG_LCD_SSD1306_SPI
FAR struct lcd_dev_s *ssd1306_initialize(FAR struct spi_dev_s *dev,
                                         FAR const struct ssd1306_priv_s *board_priv,
                                         unsigned int devno);
#else
FAR struct lcd_dev_s *ssd1306_initialize(FAR struct i2c_master_s *dev,
                                         FAR const struct ssd1306_priv_s *board_priv,
                                         unsigned int devno);
#endif

/************************************************************************************************
 * Name:  ssd1306_fill
 *
 * Description:
 *   This non-standard method can be used to clear the entire display by writing one
 *   color to the display.  This is much faster than writing a series of runs.
 *
 * Input Parameters:
 *   priv   - Reference to private driver structure
 *
 * Assumptions:
 *   Caller has selected the OLED section.
 *
 **************************************************************************************/

int ssd1306_fill(FAR struct lcd_dev_s *dev, uint8_t color);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_LCD_SSD1306 */
#endif /* __INCLUDE_NUTTX_SSD1306_H */
