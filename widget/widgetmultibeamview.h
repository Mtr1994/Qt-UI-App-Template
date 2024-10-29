#ifndef WIDGETMULTIBEAMVIEW_H
#define WIDGETMULTIBEAMVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_0>
#include <QOpenGLShaderProgram>

class WidgetMultiBeamView : public QOpenGLWidget, public QOpenGLFunctions_3_0
{
    Q_OBJECT
public:
    explicit WidgetMultiBeamView(QWidget *parent = nullptr);

private:
    void init();
    void focusView();

public:
    // test
    void test2();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPointF mapToOpenGL(const QPointF &point);

    void drawPoints();
    void drawTest();

private slots:
    void slot_recv_multibeam_bath_data(uint32_t number, float* points, const std::vector<double> &ranges);

private:
    // 着色器
    QOpenGLShaderProgram mShaderProgram;
    // VAO
    unsigned int mVAOArray[3];
    // VBO
    unsigned int mVBOArray[3];
    // EBO
    unsigned int mEBOArray[3];
    // 着色器状态
    bool mShaderProgramLinked = false;
    // 拖动
    QPointF mLastMousePosition;
    bool mMousePressed = false;
    QPointF mCurrentMousePosition;
    // 缩放比例
    double mViewScale = 1;
    // 位移
    double mOffsetX = 0;
    double mOffsetY = 0;
    // 地形原始数据缓存
    std::vector<uint32_t> mMeshIndices;
    uint32_t mEndBeamPointNumber = 0;
    uint32_t mBathyPointNumber = 0;
    float *mBathyPointData = nullptr;

    std::vector<double> mBathyDataRange;

    bool mTestFlag = false;
};

#endif // WIDGETMULTIBEAMVIEW_H
