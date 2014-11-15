#include <algorithm>
#include <cstring>
#include "Common/cpuinfo.h"
#include "Common/pixel.h"
#include "depth_convert.h"
#include "quantize.h"

namespace zimg {;
namespace depth {;

namespace {;

template <class T>
IntegerToFloat<T> make_integer_to_float(const PixelFormat &fmt)
{
	return{ fmt.depth, fmt.fullrange, fmt.chroma };
}

template <class T>
FloatToInteger<T> make_float_to_integer(const PixelFormat &fmt)
{
	return{ fmt.depth, fmt.fullrange, fmt.chroma };
}

class DepthConvertC : public DepthConvert {
public:
	void byte_to_half(const uint8_t *src, uint16_t *dst, int width, const PixelFormat &src_fmt) const override
	{
		auto cvt = make_integer_to_float<uint8_t>(src_fmt);

		std::transform(src, src + width, dst, [=](uint8_t x) { return depth::float_to_half(cvt(x)); });
	}

	void byte_to_float(const uint8_t *src, float *dst, int width, const PixelFormat &src_fmt) const override
	{
		std::transform(src, src + width, dst, make_integer_to_float<uint8_t>(src_fmt));
	}

	void word_to_half(const uint16_t *src, uint16_t *dst, int width, const PixelFormat &src_fmt) const override
	{
		auto cvt = make_integer_to_float<uint16_t>(src_fmt);

		std::transform(src, src + width, dst, [=](uint16_t x) { return depth::float_to_half(cvt(x)); });
	}

	void word_to_float(const uint16_t *src, float *dst, int width, const PixelFormat &src_fmt) const override
	{
		std::transform(src, src + width, dst, make_integer_to_float<uint16_t>(src_fmt));
	}

	void half_to_float(const uint16_t *src, float *dst, int width) const override
	{
		std::transform(src, src + width, dst, depth::half_to_float);
	}

	void float_to_half(const float *src, uint16_t *dst, int width) const override
	{
		std::transform(src, src + width, dst, depth::float_to_half);
	}
};

} // namespace


DepthConvert::~DepthConvert()
{
}

DepthConvert *create_depth_convert(CPUClass cpu)
{
	return new DepthConvertC{};
}

} // namespace depth
} // namespace zimg
