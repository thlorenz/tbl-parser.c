#ifndef __TBL_PARSER_H__
#define __TBL_PARSER_H__

#define CHECK(r, msg) if (r) {                                                            \
  fprintf(stderr, "%s: [%s(%ld): %s]\n", msg, uv_err_name((r)), (long)r, uv_strerror((r))); \
  exit(1);                                                                                \
}

#define UVERR(r, msg) fprintf(stderr, "%s: [%s(%ld): %s]\n", msg, uv_err_name((r)), (long)r, uv_strerror((r)));

#endif
