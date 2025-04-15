#include <string.h>
#include <cmath>
#include "../include/VertexShader.h"
namespace SoftRender {

	enum GeConstruct { kTrivial };

	struct Matrix {
		float rc[4][4]; // get row, get column (тво€ структура)

		//  онструктор по умолчанию Ч identity
		Matrix() {
			setToIdentity();
		}

		// —пециальный "пустой" конструктор
		Matrix(GeConstruct) {
			// ни хера не делает Ч rc остаЄтс€ как есть
		}

		void setToIdentity() {
			memset(rc, 0, sizeof(rc));
			for (int i = 0; i < 4; ++i)
				rc[i][i] = 1.0f;
		}

		static Matrix identity() {
			return Matrix();
		}

		static Matrix translation(float x, float y, float z) {
			Matrix mat(kTrivial);
			memset(mat.rc, 0, sizeof(mat.rc));
			for (int i = 0; i < 4; ++i) mat.rc[i][i] = 1.0f;
			mat.rc[0][3] = x;
			mat.rc[1][3] = y;
			mat.rc[2][3] = z;
			return mat;
		}

		static Matrix scaling(float sx, float sy, float sz) {
			Matrix mat(kTrivial);
			memset(mat.rc, 0, sizeof(mat.rc));
			mat.rc[0][0] = sx;
			mat.rc[1][1] = sy;
			mat.rc[2][2] = sz;
			mat.rc[3][3] = 1.0f;
			return mat;
		}

		static Matrix rotation(float angle) {
			Matrix mat = identity();
			mat.rc[0][0] = cos(angle);
			mat.rc[0][2] = sin(angle);
			mat.rc[2][0] = -sin(angle);
			mat.rc[2][2] = cos(angle);
			return mat;
		}

		static Matrix perspective(float fov, float aspect, float znear, float zfar) {
			Matrix mat(kTrivial);
			memset(mat.rc, 0, sizeof(mat.rc));
			float f = 1.0f / tan(fov / 2.0f);
			mat.rc[0][0] = f / aspect;
			mat.rc[1][1] = f;
			mat.rc[2][2] = (zfar + znear) / (znear - zfar);
			mat.rc[2][3] = (2 * zfar * znear) / (znear - zfar);
			mat.rc[3][2] = -1.0f;
			return mat;
		}

		static Matrix screenspace(float width, float height) {
			Matrix mat = identity();
			mat.rc[0][0] = width / 2.0f;
			mat.rc[1][1] = -height / 2.0f;
			mat.rc[0][3] = width / 2.0f;
			mat.rc[1][3] = height / 2.0f;
			return mat;
		}

		Matrix operator*(const Matrix& other) const {
			Matrix result(kTrivial);
			memset(result.rc, 0, sizeof(result.rc));
			for (int i = 0; i < 4; ++i)
				for (int j = 0; j < 4; ++j)
					for (int k = 0; k < 4; ++k)
						result.rc[i][j] += rc[i][k] * other.rc[k][j];
			return result;
		}
	};

	struct Vector4 {
		float x, y, z, w;

		Vector4 operator*(const Matrix& mat) const {
			Vector4 r;
			r.x = x * mat.rc[0][0] + y * mat.rc[0][1] + z * mat.rc[0][2] + w * mat.rc[0][3];
			r.y = x * mat.rc[1][0] + y * mat.rc[1][1] + z * mat.rc[1][2] + w * mat.rc[1][3];
			r.z = x * mat.rc[2][0] + y * mat.rc[2][1] + z * mat.rc[2][2] + w * mat.rc[2][3];
			r.w = x * mat.rc[3][0] + y * mat.rc[3][1] + z * mat.rc[3][2] + w * mat.rc[3][3];
			return r;
		}
	};

	template<typename T>
	struct RasterizerVertex {
		T location;
	};

	class VertexShader {
	public:
		VertexShader(int width, int height) : width_(width), height_(height) {}

		Matrix& modelToScreen(Matrix& matrixToCreate) {
			//Matrix transform = Matrix::translation(0.05f, 0.05f, -20.0f);
			Matrix rotate = Matrix::rotation(0.025f);
			//Matrix scale = Matrix::scaling(100.0f, 100.0f, 100.0f);
			Matrix perspective = Matrix::perspective(3.14f / 1.5f, float(width_) / float(height_), 0.01f, 1000.0f);
			Matrix screen = Matrix::screenspace(float(width_), float(height_));
			Matrix scale = Matrix::scaling(1.0f, 1.0f, 1.0f);
			Matrix transform = Matrix::translation(0.0f, 0.0f, -5.0f);  // ближе к экрану
			matrixToCreate = screen * perspective * transform * rotate * scale;
			return matrixToCreate;
		}

		void transform_triangle(RasterizerVertex<Vector4> vec4[3], const Matrix& m) {
			for (int i = 0; i < 3; ++i)
				vec4[i].location = vec4[i].location * m;
		}

	private:
		int width_, height_;
	};

} // namespace SoftRender