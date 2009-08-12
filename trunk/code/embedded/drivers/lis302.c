#include <io.h>
#include <signal.h>

#include "spi.h"
#include "lis302.h"

#define READ_BIT 0x80
#define MULTIPLE_BIT 0x40

#define REG_WHOAMI  0xF
#define REG_CTRL1   0x20
#define REG_CTRL2   0x21
#define REG_CTRL3   0x22
#define REG_STATUS  0x27
#define REG_OUTX    0x29
#define REG_OUTY    0x2B
#define REG_OUTZ    0x2D

#define CTRL1_400HZ 0x80 // 400Hz data rate, otherwise 100Hz
#define CTRL1_EN    0x40 // Enable, otherwise Power Down
#define CTRL1_FS    0x20 // Full Scale (+/-8g), otherwise +/-2g
#define CTRL1_ZEN   0x04 // Z enable
#define CTRL1_YEN   0x02 // Y enable
#define CTRL1_XEN   0x01 // X enable

uint8_t iam;

static void inline write_reg(uint8_t addr, uint8_t value);
static uint8_t inline read_reg(uint8_t addr);

void lis302_init(void) {
  spi_init();
  
  write_reg(REG_CTRL2, 0x40);
  write_reg(REG_CTRL1, CTRL1_EN | CTRL1_XEN | CTRL1_YEN | CTRL1_ZEN);
  iam = read_reg(REG_CTRL1);
  
  iam = read_reg(REG_STATUS);
  iam = read_reg(REG_OUTX);
  
}

int8_t lis302_getx(void) {
  int a = read_reg(REG_OUTX);
  return a > 127 ? a-256:a;
}
int8_t lis302_gety(void) {
  int a = read_reg(REG_OUTY);
  return a > 127 ? a-256:a;
}
int8_t lis302_getz(void) {
  int a = read_reg(REG_OUTZ);
  return a > 127 ? a-256:a;
}

static void inline write_reg(uint8_t addr, uint8_t value) {
  spi_accel_select();
  spi_write_single(addr & 0x3F);
  spi_write_single(value);
  spi_accel_deselect();
}
static uint8_t inline read_reg(uint8_t addr) {
  uint8_t ret;
  spi_accel_select();
  spi_write_single( (addr & 0x3F) | READ_BIT);
  ret = spi_read_single();
  spi_accel_deselect();
  return ret;
}
  
