#include "libuwebsockets.h"

#include "../uWebSockets/src/App.h"
#include <string_view>

#pragma region uWS-app

#define METHOD(name)                                                                                                        \
    void uws_app_##name(uws_app_t *app, const char *pattern, uws_method_handler handler)                                    \
    {                                                                                                                       \
        ((uWS::App *)app)->name(pattern, [handler](auto *res, auto *req) { handler((uws_res_t *)res, (uws_req_t *)req); }); \
    };
LOAD_METHODS
#undef METHOD

// Identical structure to Nelua
struct nlstring
{
  uint8_t* data;
  unsigned long size;
};

std::string_view uws_c_string_view_to_cpp(const uws_c_string_view_t* ptr) {
  return std::string_view((char*) ptr->data, ptr->size);
}

uws_c_string_view_t uws_cpp_string_view_to_c(const std::string_view& view) {
  return {
    (uint8_t*) view.data(),
    view.length()
  };
}

uws_app_t *uws_create_app()
{
    return (uws_app_t *)new uWS::App();
}

void uws_app_destroy(uws_app_t *app)
{
    delete ((uWS::App *)app);
}

void uws_app_run(uws_app_t *app)
{
    ((uWS::App *)app)->run();
}

void uws_app_listen(uws_app_t *app, int port, uws_listen_handler handler)
{
    ((uWS::App *)app)->listen(port, [handler](struct us_listen_socket_t *listen_socket)
                              { handler((struct us_listen_socket_t *)listen_socket); });
}

void uws_app_close(uws_app_t *app)
{
    ((uWS::App *)app)->close();
}

#pragma endregion
#pragma region uWS-Response

void uws_res_close(uws_res_t *res)
{
    ((uWS::HttpResponse<false> *)res)->close();
}

void uws_res_end(uws_res_t *res, const uws_c_string_view_t* view, bool close_connection)
{
    ((uWS::HttpResponse<false> *)res)->end(uws_c_string_view_to_cpp(view), close_connection);
}

void uws_res_cork(uws_res_t *res, void (*callback)(uws_res_t *res))
{
    ((uWS::HttpResponse<false> *)res)->cork([callback, res]()
                                            { callback(res); });
}

void uws_res_pause(uws_res_t *res)
{
    ((uWS::HttpResponse<false> *)res)->pause();
}

void uws_res_resume(uws_res_t *res)
{
    ((uWS::HttpResponse<false> *)res)->resume();
}

void uws_res_write_continue(uws_res_t *res)
{
    ((uWS::HttpResponse<false> *)res)->writeContinue();
}

void uws_res_write_status(uws_res_t *res, const uws_c_string_view_t *status)
{
    ((uWS::HttpResponse<false> *)res)->writeStatus(uws_c_string_view_to_cpp(status));
}

void uws_res_write_header(uws_res_t *res, const uws_c_string_view_t *key, const uws_c_string_view_t *value)
{
    ((uWS::HttpResponse<false> *)res)->writeHeader(uws_c_string_view_to_cpp(key), uws_c_string_view_to_cpp(value));
}

void uws_res_write_header_int(uws_res_t *res, const uws_c_string_view_t *key, uint64_t value)
{
    ((uWS::HttpResponse<false> *)res)->writeHeader(uws_c_string_view_to_cpp(key), value);
}

void uws_res_end_without_body(uws_res_t *res, bool close_connection)
{
    ((uWS::HttpResponse<false> *)res)->endWithoutBody(std::nullopt, close_connection);
}

bool uws_res_write(uws_res_t *res, const uws_c_string_view_t *data)
{
    return ((uWS::HttpResponse<false> *)res)->write(uws_c_string_view_to_cpp(data));
}

void uws_res_override_write_offset(uws_res_t *res, uintmax_t offset)
{
    ((uWS::HttpResponse<false> *)res)->overrideWriteOffset(offset);
}

bool uws_res_has_responded(uws_res_t *res)
{
    return ((uWS::HttpResponse<false> *)res)->hasResponded();
}

void uws_res_on_writable(uws_res_t *res, uws_res_on_writable_handler handler)
{
    ((uWS::HttpResponse<false> *)res)->onWritable([handler, res](uintmax_t a)
                                                  { return handler(res, a); });
}

void uws_res_on_aborted(uws_res_t *res, uws_res_on_aborted_handler handler)
{
    ((uWS::HttpResponse<false> *)res)->onAborted([handler, res]
                                                 { handler(res); });
}

void uws_res_on_data(uws_res_t *res, uws_res_on_data_handler handler)
{
    ((uWS::HttpResponse<false> *)res)->onData([handler, res](auto chunk, bool is_end)
                                              { handler(res, uws_cpp_string_view_to_c(chunk), is_end); });
}

void uws_res_upgrade(uws_res_t *res, uws_req_t *req, void *data, uws_socket_context_t *ws)
{
    uWS::HttpRequest *request = (uWS::HttpRequest *)req;
    ((uWS::HttpResponse<false> *)res)->template upgrade<void *>(
        data ? std::move(data) : NULL,
        request->getHeader("sec-websocket-key"),
        request->getHeader("sec-websocket-protocol"),
        request->getHeader("sec-websocket-extensions"),
        (us_socket_context_t *)ws
    );
}

uws_try_end_result_t uws_res_try_end(uws_res_t *res, const uws_c_string_view_t *data, uintmax_t total_size, bool close_connection)
{

    std::pair<bool, bool> result = ((uWS::HttpResponse<false> *)res)->tryEnd(uws_c_string_view_to_cpp(data), total_size);
    return uws_try_end_result_t{
        .ok = result.first,
        .has_responded = result.second,
    };
}

uintmax_t uws_res_get_write_offset(uws_res_t *res)
{
    return ((uWS::HttpResponse<false> *)res)->getWriteOffset();
}

uws_c_string_view_t uws_res_get_remote_address(uws_res_t *res)
{
    return uws_cpp_string_view_to_c(((uWS::HttpResponse<false> *)res)->getRemoteAddress());
}

uws_c_string_view_t uws_res_get_remote_address_as_text(uws_res_t *res)
{
    return uws_cpp_string_view_to_c(((uWS::HttpResponse<false> *)res)->getRemoteAddressAsText());
}

#pragma endregion
#pragma region uWS-Request

bool uws_req_is_ancient(uws_req_t *res)
{
    return ((uWS::HttpRequest *)res)->isAncient();
}

bool uws_req_get_yield(uws_req_t *res)
{
    return ((uWS::HttpRequest *)res)->getYield();
}

void uws_req_set_yield(uws_req_t *res, bool yield)
{
    return ((uWS::HttpRequest *)res)->setYield(yield);
}

uws_c_string_view_t uws_req_get_url(uws_req_t *res)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getUrl());
}

uws_c_string_view_t uws_req_get_full_url(uws_req_t *res)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getFullUrl());
}

uws_c_string_view_t uws_req_get_case_sensitive_method(uws_req_t *res)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getCaseSensitiveMethod());
}

uws_c_string_view_t uws_req_get_header(uws_req_t *res, const uws_c_string_view_t *lower_case_header)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getHeader(uws_c_string_view_to_cpp(lower_case_header)));
}

uws_c_string_view_t uws_req_get_query(uws_req_t *res, const uws_c_string_view_t *key)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getQuery(uws_c_string_view_to_cpp(key)));
}

uws_c_string_view_t uws_req_get_parameter_name(uws_req_t *res, const uws_c_string_view_t *key)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getParameter(uws_c_string_view_to_cpp(key)));
}

uws_c_string_view_t uws_req_get_parameter_index(uws_req_t *res, unsigned short index)
{
    return uws_cpp_string_view_to_c(((uWS::HttpRequest *)res)->getParameter(index));
}

#pragma endregion
#pragma region uWS-Websockets

#define WEBSOCKET_HANDLER(field, lambda_args, lambda_body)         \
    if (behavior.field)                                            \
    {                                                              \
        auto handler = behavior.field;                             \
        generic_handler.field = [handler] lambda_args lambda_body; \
    }

void uws_ws(uws_app_t *app, const char *pattern, uws_socket_behavior_t behavior)
{
    auto generic_handler = uWS::App::WebSocketBehavior<void *>{
        .compression = (uWS::CompressOptions)(uint64_t)behavior.compression,
        .maxPayloadLength = behavior.maxPayloadLength,
        .idleTimeout = behavior.idleTimeout,
        .maxBackpressure = behavior.maxBackpressure,
        .closeOnBackpressureLimit = behavior.closeOnBackpressureLimit,
        .resetIdleTimeoutOnSend = behavior.resetIdleTimeoutOnSend,
        .sendPingsAutomatically = behavior.sendPingsAutomatically,
        .maxLifetime = behavior.maxLifetime,
    };

    WEBSOCKET_HANDLER(upgrade, (auto *res, auto *req, auto *context), {
        handler((uws_res_t *)res, (uws_req_t *)req, (uws_socket_context_t *)context);
    });

    WEBSOCKET_HANDLER(open, (auto *ws), {
        handler((uws_websocket_t *)ws);
    });

    WEBSOCKET_HANDLER(message, (auto *ws, auto message, auto opcode), {
        handler((uws_websocket_t *)ws, uws_cpp_string_view_to_c(message), (uws_opcode_t)opcode);
    });

    WEBSOCKET_HANDLER(dropped, (auto *ws, auto message, auto opcode), {
        handler((uws_websocket_t *)ws, uws_cpp_string_view_to_c(message), (uws_opcode_t)opcode);
    });

    WEBSOCKET_HANDLER(drain, (auto *ws), {
        handler((uws_websocket_t *)ws);
    });

    WEBSOCKET_HANDLER(ping, (auto *ws, auto message), {
        handler((uws_websocket_t *)ws, uws_cpp_string_view_to_c(message));
    });

    WEBSOCKET_HANDLER(pong, (auto *ws, auto message), {
        handler((uws_websocket_t *)ws, uws_cpp_string_view_to_c(message));
    });

    WEBSOCKET_HANDLER(close, (auto *ws, int code, auto message), {
        handler((uws_websocket_t *)ws, code, uws_cpp_string_view_to_c(message));
    });

    WEBSOCKET_HANDLER(subscription, (auto *ws, auto topic, int subscribers, int old_subscribers), {
        handler((uws_websocket_t *)ws, uws_cpp_string_view_to_c(topic), subscribers, old_subscribers);
    });

    uWS::App *uwsApp = (uWS::App *)app;
    uwsApp->ws<void *>(pattern, std::move(generic_handler));
}

void uws_ws_close(uws_websocket_t *ws)
{
    ((uWS::WebSocket<false, true, void *> *)ws)->close();
}

uws_sendstatus_t uws_ws_send(uws_websocket_t *ws, const nlstring *message, uws_opcode_t opcode)
{
    return (uws_sendstatus_t)((uWS::WebSocket<false, true, void *> *)ws)->send(uws_c_string_view_to_cpp(message), (uWS::OpCode)(unsigned char)opcode);
}

uws_sendstatus_t uws_ws_send_with_options(uws_websocket_t *ws, const nlstring *message, uws_opcode_t opcode, bool compress, bool fin)
{
    return (uws_sendstatus_t)((uWS::WebSocket<false, true, void *> *)ws)->send(uws_c_string_view_to_cpp(message), (uWS::OpCode)(unsigned char)opcode, compress, fin);
}

uws_sendstatus_t uws_ws_send_fragment(uws_websocket_t *ws, const nlstring *message, bool compress)
{
    return (uws_sendstatus_t)((uWS::WebSocket<false, true, void *> *)ws)->sendFragment(uws_c_string_view_to_cpp(message), compress);
}

uws_sendstatus_t uws_ws_send_first_fragment(uws_websocket_t *ws, const nlstring *message, bool compress)
{
    return (uws_sendstatus_t)((uWS::WebSocket<false, true, void *> *)ws)->sendFirstFragment(uws_c_string_view_to_cpp(message), uWS::OpCode::BINARY, compress);
}

uws_sendstatus_t uws_ws_send_first_fragment_with_opcode(uws_websocket_t *ws, const nlstring *message, uws_opcode_t opcode, bool compress)
{
    return (uws_sendstatus_t)((uWS::WebSocket<false, true, void *> *)ws)->sendFirstFragment(uws_c_string_view_to_cpp(message), (uWS::OpCode)(unsigned char)opcode, compress);
}

uws_sendstatus_t uws_ws_send_last_fragment(uws_websocket_t *ws, const nlstring *message, bool compress)
{
    return (uws_sendstatus_t)((uWS::WebSocket<false, true, void *> *)ws)->sendLastFragment(uws_c_string_view_to_cpp(message), compress);
}

void uws_ws_end(uws_websocket_t *ws, int code, const nlstring *message)
{
    ((uWS::WebSocket<false, true, void *> *)ws)->end(code, uws_c_string_view_to_cpp(message));
}

void uws_ws_cork(uws_websocket_t *ws, void (*handler)(uws_websocket_t *ws))
{
    ((uWS::WebSocket<false, true, void *> *)ws)->cork([handler, ws]()
              { handler(ws); });
}

bool uws_ws_subscribe(uws_websocket_t *ws, const nlstring *topic)
{
    return ((uWS::WebSocket<false, true, void *> *)ws)->subscribe(uws_c_string_view_to_cpp(topic));
}

bool uws_ws_unsubscribe(uws_websocket_t *ws, const nlstring *topic)
{
    return ((uWS::WebSocket<false, true, void *> *)ws)->unsubscribe(uws_c_string_view_to_cpp(topic));
}

bool uws_ws_is_subscribed(uws_websocket_t *ws, const nlstring *topic)
{
    return ((uWS::WebSocket<false, true, void *> *)ws)->isSubscribed(uws_c_string_view_to_cpp(topic));
}

bool uws_ws_publish(uws_websocket_t *ws, const nlstring *topic, const nlstring *message)
{
    return ((uWS::WebSocket<false, true, void *> *)ws)->publish(uws_c_string_view_to_cpp(topic), uws_c_string_view_to_cpp(message));
}

bool uws_ws_publish_with_options(uws_websocket_t *ws, const nlstring *topic, const nlstring *message, uws_opcode_t opcode, bool compress)
{
    return ((uWS::WebSocket<false, true, void *> *)ws)->publish(uws_c_string_view_to_cpp(topic), uws_c_string_view_to_cpp(message), (uWS::OpCode)(unsigned char)opcode, compress);
}

unsigned int uws_ws_get_buffered_amount(uws_websocket_t *ws)
{
    uWS::WebSocket<false, true, void *> *uws = (uWS::WebSocket<false, true, void *> *)ws;
    return uws->getBufferedAmount();
}

nlstring uws_ws_get_remote_address(uws_websocket_t *ws)
{
    return uws_cpp_string_view_to_c(((uWS::WebSocket<false, true, void *> *)ws)->getRemoteAddress());
}

nlstring uws_ws_get_remote_address_as_text(uws_websocket_t *ws)
{
    return uws_cpp_string_view_to_c(((uWS::WebSocket<false, true, void *> *)ws)->getRemoteAddressAsText());
}

#pragma endregion

void uws_loop_defer(us_loop_t *loop, void(cb()))
{
    ((uWS::Loop *)loop)->defer([cb]()
                               { cb(); });
}

struct us_loop_t *uws_get_loop()
{
    return (struct us_loop_t *)uWS::Loop::get();
}

struct us_loop_t *uws_get_loop_with_native(void *existing_native_loop)
{
    return (struct us_loop_t *)uWS::Loop::get(existing_native_loop);
}
