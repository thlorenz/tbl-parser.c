#include <stdio.h>
#include <stdlib.h>

#include <log.h>
#include <uv.h>

#include <tbl-parser.h>

#define NOIPC 0

static void on_alloc     ( uv_handle_t *handle, size_t size, uv_buf_t *buf);
static void on_fs_open   ( uv_fs_t* fs_open_req);
static void pipe_file    ( uv_loop_t* loop, uv_file file);
static void on_file_read ( uv_stream_t *file_pipe, ssize_t nread, const uv_buf_t *buf);

void on_alloc(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  *buf = uv_buf_init((char*) malloc(size), size);
}

int tblp_parse_file(uv_loop_t* loop, const char* file_path) {
  uv_fs_t *fs_open_req = (uv_fs_t*) malloc(sizeof(uv_fs_t));
  return uv_fs_open(loop, fs_open_req, file_path, O_RDONLY, 0644, on_fs_open);
}

static void on_fs_open(uv_fs_t* fs_open_req) {
  CHECK(fs_open_req->result, "async open");

  uv_loop_t *loop = fs_open_req->loop;
  uv_file file = fs_open_req->file;
  uv_fs_req_cleanup(fs_open_req);
  free(fs_open_req);

  pipe_file(loop, file);
}

static void pipe_file(uv_loop_t* loop, uv_file file) {
  int r;
  uv_pipe_t *file_pipe = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
  r = uv_pipe_init(loop, file_pipe, NOIPC);
  CHECK(r, "initializing file pipe");

  r = uv_pipe_open(file_pipe, file);
  CHECK(r, "opening file pipe");

  r = uv_read_start((uv_stream_t*) file_pipe, on_alloc, on_file_read);
  CHECK(r, "starting to read file pipe");
}

static void on_file_read(uv_stream_t *file_pipe, ssize_t nread, const uv_buf_t *buf) {
  log_debug("read: %s", buf->base);
}


int main(void) {
  uv_loop_t *loop;
  loop = uv_default_loop();

  log_info("testing tbl-parser");

  tblp_parse_file(loop, "src/tbl-parser.c");

  uv_run(loop, UV_RUN_DEFAULT);
  return 0;
}
