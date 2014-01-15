#include <log.h>
#include <uv.h>

int main(void) {
  uv_loop_t *loop;
  loop = uv_default_loop();

  log_info("testing tbl-parser");

  return 0;
}
