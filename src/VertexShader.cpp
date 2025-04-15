#include <string.h>
#include <cmath>
#include "../include/VertexShader.h"
#include "../include/MathUtils.h"


namespace SoftRender {

	struct Vector4 {
		float x, y, z, w;

		Vector4 operator *(const MathUtils::Matrix& mat) const {
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

		MathUtils::Matrix& modelToScreen(MathUtils::Matrix& matrixToCreate) {
			//Matrix transform = Matrix::translation(0.05f, 0.05f, -20.0f);
			MathUtils::Matrix rotate = MathUtils::Matrix::rotation(0.025f);
			//Matrix scale = Matrix::scaling(100.0f, 100.0f, 100.0f);
			MathUtils::Matrix perspective = MathUtils::Matrix::perspective(3.14f / 1.5f, float(width_) / float(height_), 0.01f, 1000.0f);
			MathUtils::Matrix screen = MathUtils::Matrix::screenspace(float(width_), float(height_));
			MathUtils::Matrix scale = MathUtils::Matrix::scaling(1.0f, 1.0f, 1.0f);
			MathUtils::Matrix transform = MathUtils::Matrix::translation(0.0f, 0.0f, -5.0f);  // ближе к экрану
			matrixToCreate = screen * perspective * transform * rotate * scale;
			return matrixToCreate;
		}

		void transform_triangle(RasterizerVertex<Vector4> vec4[3], const MathUtils::Matrix& m) {
			for (int i = 0; i < 3; ++i)
				vec4[i].location = vec4[i].location * m;
		}

	private:
		int width_, height_;
	};

} // namespace SoftRender

