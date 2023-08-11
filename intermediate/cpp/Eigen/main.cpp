#include <Eigen/Dense>

template <typename ScalarType, int Dimension>
class Vector : public Eigen::Matrix<ScalarType, Dimension, 1> {
public:
	using Base = Eigen::Matrix<ScalarType, Dimension, 1>;

public:
	Vector(float x, float y, float z, float w) : Base(x, y, z, w) {
	}

	Vector(const Base& src) {
	}

	template <typename Derived>
	Vector(const Eigen::MatrixBase<Derived>& other) : Base(other) {
	}

	Vector(std::initializer_list<ScalarType> list) : Base(list) {
	}

	using Base::operator=;
};

using Vector4f = Vector<float, 4>;
using Matrix44f = Eigen::Matrix<float, 4, 4>;

int main() {
	// Matrix44f transform;
	// Vector4f v(10.0f, 10.0f, 10.0f, 10.0f);
	// Vector4f v2{10.0f, 10.0f, 10.0f, 10.0f};
	float va[4] = {1.0f, 2.0f, 3.0f, 4.0f};
	Eigen::Matrix<float, 4, 1> v4{va};
	// Vector4f v3{va};
	// Vector4f vt = transform * v;
}