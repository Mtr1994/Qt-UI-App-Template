#include "widgetmultibeamview.h"
#include "log/applogger.h"
#include "public/appsignal.h"

#include <QWheelEvent>
#include <QImage>
#include <QVector4D>

// test
#include <QDebug>

WidgetMultiBeamView::WidgetMultiBeamView(QWidget *parent)
    : QOpenGLWidget{parent}
{
    init();
}

void WidgetMultiBeamView::init()
{
    // float *a = new float[3]; a[0] = 1.0f; a[1] = 2.0f; a[2] = 3.0f;
    // float *b = new float[3]; b[0] = 4.0f; b[1] = 5.0f; b[2] = 6.0f;
    // b = a;
    // LOG_DEBUG("b {} {} {}", b[0], b[1], b[2]);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_multibeam_bath_data, this, &WidgetMultiBeamView::slot_recv_multibeam_bath_data);

    mViewBoundingRect = QRectF(-1, -1, 2, 2);
}

void WidgetMultiBeamView::focusView()
{
    QRectF rect = boundingRect();
     LOG_INFO("bounding rect is {} {} {} {}", rect.x(), rect.y(), rect.width(), rect.height());
    if ((rect.width() == 0) && (rect.height() == 0))
    {
        mViewScale = 1.0;
        mOffsetX = 0; mOffsetY = 0;
        mViewBoundingRect = QRectF(-1, -1, 2, 2);
    }
    else
    {
        mViewBoundingRect = rect;
    }
    update();
}

void WidgetMultiBeamView::test2()
{
    LOG_DEBUG("offset {} {} {}", mOffsetX, mOffsetY, mViewScale);
}

void WidgetMultiBeamView::initializeGL()
{
    // 不调用这句话，就不能使用 gl 开头的函数，程序会崩溃
    bool status = true;
    status = initializeOpenGLFunctions();
    //emit AppSignal::getInstance()->sgl_init_opengl_status(status);
    if (!status)
    {
        mShaderProgramLinked = false;
        //请检查当前使用的显卡类型
        LOG_DEBUG("The current graphics card does not support it, and the page initialization failed. Please switch to a separate graphics card and restart.");
        return;
    }

    LOG_DEBUG("OpenGL Version is {}", glGetString(GL_VERSION));

    // 着色器文件不能使用 UTF-8-BOM 编码，会报错，只能采用 UTF-8 编码

    // 加载顶点着色器
    status = mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resource/shader/shader.vert");
    if (!status)
    {
        LOG_DEBUG("parse vertex shader fail ", mShaderProgram.log().toStdString());
        return;
    }

    // 加载片段着色器
    status= mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resource/shader/shader.frag");
    if (!status)
    {
        LOG_DEBUG("parse fragment shader fail ", mShaderProgram.log().toStdString());
        return;
    }

    // 链接程序
    mShaderProgramLinked = mShaderProgram.link();
    if (!mShaderProgramLinked)
    {
        LOG_DEBUG("link shader fail ", mShaderProgram.log().toStdString());
        return;
    }

    // 画 纹理 （图片）
    {
        glUseProgram(mShaderProgram.programId());
        glGenVertexArrays(3, mVAOArray);
        glGenBuffers(3, mVBOArray);
        glGenBuffers(3, mEBOArray);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        //glEnable(GL_DEPTH_TEST);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // 初始化颜色
    QImage imageA("colorA.png");
    QVector<QVector4D> colormap(256);
    for (int i = 0; i < 256; i++)
    {
        QColor color = imageA.pixelColor(i, 5);
        colormap[i] = { color.red() * 1.0f / 255, color.green() * 1.0f / 255, color.blue() * 1.0f / 255, 1.0f};
    }

    mShaderProgram.setUniformValue("minDepth", -2000.0f);
    mShaderProgram.setUniformValue("maxDepth", -3000.0f);
    mShaderProgram.setUniformValueArray("colormap", colormap.constData(), 256);

    LOG_DEBUG("opengl init success.");
}

void WidgetMultiBeamView::resizeGL(int w, int h)
{
    // Z 轴正方向则指向观察者（或屏幕外)
    QMatrix4x4 proj;
    //proj.ortho(w * -0.5, w * 0.5, h * -0.5, h * 0.5, 10000.0f, -120000.0f);
    proj.perspective(45.0f, (float)w / h, 0.01f, 10000.0f);
    mShaderProgram.setUniformValue("proj", proj);
}

void WidgetMultiBeamView::paintGL()
{
    if (!mShaderProgramLinked) return;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float cx = mViewBoundingRect.center().x();
    float cy = mViewBoundingRect.center().y();

    // 判断是否绘制网格
    QMatrix4x4 view;
    view.lookAt(QVector3D(cx * mViewScale , cy * mViewScale, mMinBathyDepth * -1.0), QVector3D(cx * mViewScale, cy * mViewScale, mMinBathyDepth), QVector3D(0, 1, 0));
    view.scale(mViewScale);
    view.rotate(mOffsetX * 0.2, 0.0f, 1.0f, 0.0f);
    mShaderProgram.setUniformValue("view", view);

    QMatrix4x4 model;
    // model.translate(cx, cy, mMaxBathyDepth + (mMinBathyDepth - mMaxBathyDepth) * 0.5);
    // model.rotate(mOffsetX * 0.2, 0.0f, 1.0f, 0.0f);
    // model.rotate(mOffsetY * 0.2, 1.0f, 0.0f, 0.0f);
    // model.translate(-cx, -cy, -mMaxBathyDepth - (mMinBathyDepth - mMaxBathyDepth) * 0.5);

    mShaderProgram.setUniformValue("model", model);

    drawPoints();

    // 测试数据
    //drawTest();
}

void WidgetMultiBeamView::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y();
    float scaleFactor = (delta > 0) ? 1.1f : 0.9f;

    // try mouse center scale
    QPointF mouse = mapToOpenGL(event->position());
    //LOG_DEBUG("mouse = {} {} factor = {}", mouse.x(), mouse.y(), scaleFactor);
    // mOffsetX -= mouse.x() * mViewScale * (scaleFactor - 1);
    // mOffsetY -= mouse.y() * mViewScale * (scaleFactor - 1);

    mViewScale *= scaleFactor;
    update();
}

void WidgetMultiBeamView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) return;
    mLastMousePosition = event->position();
    mMousePressed = true;
}

void WidgetMultiBeamView::mouseMoveEvent(QMouseEvent *event)
{
    if (!mMousePressed) return;
    if (!event->buttons().testFlag(Qt::LeftButton)) return;
    QPointF delta = event->position() - mLastMousePosition;
    mOffsetX += (double)delta.x();
    mOffsetY -= (double)delta.y();

    // Update mouse press position for next move event
    mLastMousePosition = event->position();
    update();
}

void WidgetMultiBeamView::mouseReleaseEvent(QMouseEvent *event)
{
    mMousePressed = false;
    event->accept();
    update();
}

void WidgetMultiBeamView::keyPressEvent(QKeyEvent *event)
{
    event->accept();
    if (event->isAutoRepeat()) return;
    if (event->key() == Qt::Key_R)
    {
        // 聚焦
        focusView();
    }
}

QPointF WidgetMultiBeamView::mapToOpenGL(const QPointF &point)
{
    float x = width() * -0.5;
    float y = height() * 0.5;
    float x1 = x + point.x();
    float y1 = y - point.y();
    x1 = (x1 - mOffsetX) / mViewScale;
    y1 = (y1 - mOffsetY) / mViewScale;
    return QPointF(x1, y1);
}

QRectF WidgetMultiBeamView::boundingRect()
{
    return mBathyDataBoundingRect;
}

void WidgetMultiBeamView::drawPoints()
{
    glBindVertexArray(mVAOArray[0]);
    glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * mBathyPointNumber, mBathyPointData, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glPointSize(2.0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBOArray[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMeshIndices.size() * 4, mMeshIndices.data(), GL_DYNAMIC_DRAW);

    if (mMeshIndices.size())
    {
        // 让点是圆形的
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_POINTS, 0, mBathyPointNumber);
        glDisableVertexAttribArray(0);
        glDisable(GL_POINT_SMOOTH);
    }
    else
    {
        glLineWidth(1);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
        glDrawElements(GL_TRIANGLES, (uint32_t)mMeshIndices.size(), GL_UNSIGNED_INT, nullptr);
        glDisableVertexAttribArray(0);
        glDisable(GL_LINE_SMOOTH);
    }
}

void WidgetMultiBeamView::drawTest()
{
    //float vertices[12] = {0.0f, 0.0f, 10.0f, 7.0f, 100.0f, 100.0f, 20.0f, 12.0f, 200.0f, 200.0f, 30.0f, 24.0f};
    float vertices[3000];
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            vertices[i * 60 + j * 3 + 0] = i;
            vertices[i * 60 + j * 3 + 1] = j;
            vertices[i * 60 + j * 3 + 2] = -i - j * 6;
        }
    }

    glBindVertexArray(mVAOArray[1]);
    glBindBuffer(GL_ARRAY_BUFFER, mVBOArray[1]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * 1000, vertices, GL_DYNAMIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glPointSize(2.0);
    mShaderProgram.setUniformValue("u_Color", 0.2f, 0.7725f, 0.5254f, 1.0f);

    // 让点是圆形的
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_POINTS, 0, 1000);
    glDisableVertexAttribArray(0);
    glDisable(GL_POINT_SMOOTH);
}

void WidgetMultiBeamView::slot_recv_multibeam_bath_data(uint32_t number, float *points, const QRectF &rect, double minZ, double maxZ)
{
    //LOG_DEBUG("recv bathy data {} {}", mBathyPointNumber, number);
    if (number == 0) return;

    float *newBathyPointData = new float[(mBathyPointNumber + number) * 4];
    if (mBathyPointNumber > 0) memcpy(newBathyPointData, mBathyPointData, mBathyPointNumber * 4 * sizeof(float));
    memcpy(newBathyPointData + mBathyPointNumber * 4, points, number * 4 * sizeof(float));

    if (mEndBeamPointNumber > 0)
    {
        // 计算三角网格索引
        if (mEndBeamPointNumber > number)
        {
            // 下排三角，逆时针
            for (uint32_t i = 0; i < number; i++)
            {
                mMeshIndices.push_back(mBathyPointNumber + i);
                mMeshIndices.push_back(mBathyPointNumber + i - mEndBeamPointNumber);
                mMeshIndices.push_back(mBathyPointNumber + i - mEndBeamPointNumber + 1);
            }

            // 上排三角，逆时针
            for (uint32_t i = 0; i < number - 1; i++)
            {
                mMeshIndices.push_back(mBathyPointNumber + i);
                mMeshIndices.push_back(mBathyPointNumber + i - mEndBeamPointNumber + 1);
                mMeshIndices.push_back(mBathyPointNumber + i + 1);
            }
        }
        else
        {
            // 上排三角，逆时针
            for (uint32_t i = 0; i < mEndBeamPointNumber; i++)
            {
                mMeshIndices.push_back(mBathyPointNumber + i + 1);
                mMeshIndices.push_back(mBathyPointNumber + i);
                mMeshIndices.push_back(mBathyPointNumber + i - mEndBeamPointNumber);
            }

            // 下排三角，逆时针
            for (uint32_t i = 0; i < mEndBeamPointNumber - 1; i++)
            {
                mMeshIndices.push_back(mBathyPointNumber + i + 1);
                mMeshIndices.push_back(mBathyPointNumber + i - mEndBeamPointNumber);
                mMeshIndices.push_back(mBathyPointNumber + i - mEndBeamPointNumber + 1);
            }
        }
    }
    mBathyPointNumber += number;
    mEndBeamPointNumber = number;

    delete [] points;
    delete [] mBathyPointData;
    mBathyPointData = newBathyPointData;

    // 更新地形的范围
    mBathyDataBoundingRect = mBathyDataBoundingRect.united(rect);
    mMinBathyDepth = qMin(mMinBathyDepth, minZ);
    mMaxBathyDepth = qMax(mMaxBathyDepth, maxZ);
    update();
}
