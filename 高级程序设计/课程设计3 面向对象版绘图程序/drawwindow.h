#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QList>
#include <QTextEdit>
#include "structs.h"
#include "factory.h"

namespace Ui {
class DrawWindow;
}

class DrawWindow : public QMainWindow
{
	Q_OBJECT

	enum Dr_State //drag state, 拖动的状态
	{
		still,   //
		topoint, //画曲线，多边形时的待点击状态，旋转等操作时待点击状态
		todrag   //画直线、椭圆时的待拖动或拖动中状态
	};

	enum Prim   //当前选择的绘图模式
	{
		ellipse, //椭圆绘图
		line,    //直线绘图
		poly,    //多边形绘图
		curve,   //曲线绘图
		trans,   //平移
		rota,    //旋转
		sca,     //缩放
		clip,    //裁剪
		unchecked //未选择
	};

	struct TP //提示点
	{
		int id;   //对应的图元ID
		QPoint p; //图元的提示点
		TP(int idp, QPoint pp) {id = idp; p = pp;}
		TP() {}
	};


public:
	explicit DrawWindow(QWidget *parent = nullptr);
	~DrawWindow();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent* event);


private slots:
	void on_useLine_triggered(bool checked);

	void on_useEllipse_triggered(bool checked);

	void on_actSave_triggered(bool checked);

	void on_actReset_triggered(bool checked);

	void on_actRed_triggered(bool checked);

	void on_actGreen_triggered(bool checked);

	void on_actBlue_triggered(bool checked);

	void on_usePoly_triggered(bool checked);
	
	void on_useBezier_triggered();

	void on_useBspline_triggered();

	void on_useTrans_triggered();

	void on_useRotate_triggered();

	void on_useScale_triggered();

	void on_useClip_triggered();


private:
	Ui::DrawWindow *ui;

/*采用的策略：鼠标松开前，每监测到鼠标的移动，就：
 * 1. 把已有的图元的格子涂成白色
 * 2. 删除当前图元列表中的所有点
 * 3. 生成新的点
 * 4. 重新绘制已有的图元
 * 5. 绘制新的当前图元
 *
 *
 */
	int w;         //width, 宽度，x轴
	int h;         //height, 高度，y轴
	int unit_id;   //要添加到图元列表中的已画好的图元的ID
	int curvetype; //当前正在绘制的曲线的类型：0 = Bezier 1 = Bspline
	QPixmap hehe;    //主画布
	QPixmap hehedis; //用于显示到屏幕上的画布，在主画布的基础上多画了提示点
	QPen pen;
	QPainter painter;
	QList<Item*> item;  //图元列表
	Dr_State state;     //鼠标当前的拖动状态
	Prim prim;          //当前选择的绘图模式
	QPoint start_pos;   //“起点”
	QList<QPoint>* cur; //当前正在绘制的图元的点列表
	QList<TP> tippoint; //图元的提示点集
	QList<QPoint> ctrlpoint; //图元的控制点集
	int trindtip;       //所选择的图元在tippoint数组中的索引
	int trind;          //所选择的图元在item数组中的索引
	bool totrans;       //是否已进入平移状态
	QPoint rp;          //旋转和缩放的参考点
	bool showrp;        //是否显示参考点

	QString savename = "UI_output.bmp";

	//各个工厂
	LineFty linefty;
	ElliFty ellifty;
	PolyFty polyfty;
	CurveFty curvefty;

	//绘图算法
	Alg algo;

	//functions
	QPoint get_pos(QMouseEvent *event);
	void uncheck_all_units();
	void init_vars();
	void init_pixmap();
	void del_units();
	void draw(QPoint point);
	void draw_to_pixmap();
	void clip_line(QPoint point); //裁剪图元列表中所有线段
	int getCurrentNode(QPoint p);
	bool near_rp(QPoint p);
	void initUI(); //初始化自定义的widget
	void reset_tipp(); //旋转和缩放后，重新设置tippoint

	//自定义组件：PlainTextEdit
	QTextEdit* txtEdit;
};

#endif // DRAWWINDOW_H
