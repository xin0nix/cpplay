#pragma once

#include <cstdint>
#include <expected>
#include <span>

#include "Exchange.pb.h"

namespace exchange {

enum class DecodeVarIntError { kInvalidInput, kOverflow };

/**
 * @brief Декодирует целое число переменной длины (varint) из массива байтов.
 *
 * Эта функция декодирует целое число переменной длины (varint) из
 * предоставленного массива байтов. Ожидается, что varint закодирован в формате,
 * где старший бит (MSB) каждого байта указывает, является ли следующий байт
 * частью varint. Функция читает до 4 байтов и возвращает декодированное число.
 *
 * @param data Массив байтов, содержащий varint для декодирования.
 * @return Объект std::expected, содержащий декодированное значение uint64_t в
 * случае успеха, или ошибку @c DecodeVarIntError в случае неудачи.
 *
 * @note Функция проверяет недопустимый ввод и условия переполнения. Если ввод
 * пуст или varint превышает максимально допустимый размер (4 байта),
 * возвращается ошибка.
 */
std::expected<uint64_t, DecodeVarIntError>
decodeVarint(std::span<uint8_t> data);
} // namespace exchange
