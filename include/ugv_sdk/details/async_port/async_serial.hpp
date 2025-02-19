/*
 * async_serial.hpp
 *
 * Created on: Sep 10, 2020 12:59
 * Description:
 *
 * Copyright (c) 2020 Weston Robot Pte. Ltd.
 */

#ifndef ASYNC_SERIAL_HPP
#define ASYNC_SERIAL_HPP

#include <cstdint>
#include <memory>
#include <array>
#include <mutex>
#include <thread>
#include <functional>

#include "boost/asio.hpp"

#include "ugv_sdk/details/async_port/ring_buffer.hpp"

namespace westonrobot {
class AsyncSerial : public std::enable_shared_from_this<AsyncSerial> {
 public:
  using ReceiveCallback =
      std::function<void(uint8_t *data, const size_t bufsize, size_t len)>;

 public:
  AsyncSerial(std::string port_name, uint32_t baud_rate = 115200);
  ~AsyncSerial();

  // do not allow copy
  AsyncSerial(const AsyncSerial &) = delete;
  AsyncSerial &operator=(const AsyncSerial &) = delete;

  // Public API
  void SetBaudRate(unsigned baudrate);
  void SetHardwareFlowControl(bool enabled);

  bool Open();
  void Close();
  bool IsOpened() const;

  void SetReceiveCallback(ReceiveCallback cb) { rcv_cb_ = cb; }
  void SendBytes(const uint8_t *bytes, size_t length);

 private:
  std::string port_;
  bool port_opened_ = false;

#if ASIO_VERSION < 101200L
  boost::asio::io_service io_context_;
#else
  boost::asio::io_context io_context_;
#endif
  std::thread io_thread_;

  // serial port
  boost::asio::serial_port serial_port_;
  uint32_t baud_rate_ = 115200;
  bool hwflow_ = false;
  ReceiveCallback rcv_cb_ = nullptr;

  // tx/rx buffering
  static constexpr uint32_t rxtx_buffer_size = 1024 * 8;
  std::array<uint8_t, rxtx_buffer_size> rx_buf_;
  uint8_t tx_buf_[rxtx_buffer_size];
  RingBuffer<uint8_t, rxtx_buffer_size> tx_rbuf_;
  std::recursive_mutex tx_mutex_;
  bool tx_in_progress_ = false;

  void DefaultReceiveCallback(uint8_t *data, const size_t bufsize, size_t len);
  void ReadFromPort();
  void WriteToPort(bool check_if_busy);
};
}  // namespace westonrobot

#endif /* ASYNC_SERIAL_HPP */
