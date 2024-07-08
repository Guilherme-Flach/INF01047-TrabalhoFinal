#ifndef INTERPOLATOR_HEADER
#define INTERPOLATOR_HEADER
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float4.hpp"

struct BezierPath_Linear {
  glm::vec4 start;
  glm::vec4 end;
};

struct BezierPath_Quadratic {
  glm::vec4 start;
  glm::vec4 end;
  glm::vec4 control;
};


class Interpolator {
  protected:
    glm::vec4 start;
    glm::vec4 end;
    glm::vec4 currentPosition;
    float progressAmount;
    float duration;

    Interpolator(glm::vec4 start, glm::vec4 end, float duration);
    virtual void updateCurrentPosition() = 0;

    public:
    void progress(float time);
    void resetProgress();
    void fillProgress();

    glm::vec4 get_currentPosition();
    float get_progress();
    bool isFinished();
    
    void set_progress(float amount);
};

class LinearInterpolator : public Interpolator {
  protected:
    void updateCurrentPosition() override;
  public:
    LinearInterpolator(glm::vec4 start, glm::vec4 end, float duration);
    LinearInterpolator(BezierPath_Linear path, float duration);

    void set_path(BezierPath_Linear path);
};

class QuadraticInterpolator : public Interpolator {
  protected:
    glm::vec4 control;
    void updateCurrentPosition() override;

  public:
    QuadraticInterpolator(glm::vec4 start, glm::vec4 control, glm::vec4 end, float duration);
    QuadraticInterpolator(BezierPath_Quadratic path, float duration);

    static BezierPath_Quadratic createPath(glm::vec4 startPoint, glm::vec4 endPoint);
    
    void set_path(BezierPath_Quadratic path);
};

#endif //INTERPOLATOR