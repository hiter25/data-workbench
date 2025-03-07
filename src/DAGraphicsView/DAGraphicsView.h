﻿#ifndef DAGRAPHICSVIEW_H
#define DAGRAPHICSVIEW_H
#include <QGraphicsView>
#include "DAGraphicsViewGlobal.h"
#include "DAGraphicsItem.h"
#include "DAGraphicsViewOverlayMouseMarker.h"
class QWheelEvent;
namespace DA
{
class DAAbstractGraphicsViewAction;
/**
 * @brief 支持缩放和拖动的GraphicsView
 *
 * DAGraphicsView的特殊功能都通过@ref DA::DAAbstractGraphicsViewAction 提供
 *
 * 快捷键说明：
 * Ctrl++ 放大
 * Ctrl+- 缩小
 *
 */
class DAGRAPHICSVIEW_API DAGraphicsView : public QGraphicsView
{
	Q_OBJECT
	DA_DECLARE_PRIVATE(DAGraphicsView)
public:
	/**
	 * @brief 缩放设置
	 */
	enum ZoomFlag
	{
		ZoomNotUseWheel     = 0x0001,  ///< 不进行滚轮缩放
		ZoomUseWheel        = 0x0002,  ///< 直接用wheel就可以
		ZoomUseWheelAndCtrl = 0x0004   ///< 需要使用ctrl键
	};
	Q_DECLARE_FLAGS(ZoomFlags, ZoomFlag)
	Q_FLAG(ZoomFlag)

	/**
	 * @brief 表征拖动状态
	 */
	enum PadFlag
	{
		PadDiable                     = 0x0001,  ///< 不允许拖动
		PadByWheelMiddleButton        = 0x0002,  ///< 通过滚轮中键移动
		PadBySpaceWithMouseLeftButton = 0x0004   ///< 通过长按空格和鼠标左键实现拖动
	};
	Q_DECLARE_FLAGS(PadFlags, PadFlag)
	Q_FLAG(PadFlag)

public:
	DAGraphicsView(QWidget* parent = 0);
	DAGraphicsView(QGraphicsScene* scene, QWidget* parent = 0);
	~DAGraphicsView();
	// 设置缩放的因子
	void setScaleRange(qreal min, qreal max);
	// 获取缩放因子
	qreal getScaleMaxFactor() const;
	qreal getScaleMinFactor() const;

	// 设置是否可以滚轮缩放
	bool isEnaleWheelZoom() const;
	void setEnaleWheelZoom(bool enaleWheelZoom = true, ZoomFlags zf = ZoomUseWheelAndCtrl);

	// 获取鼠标对应的scence的位置坐标
	QPointF getMouseScenePos() const;

	// 设置缩放flag
	void setZoomFrags(ZoomFlags zf);
	ZoomFlags getZoomFlags() const;

	// 判断是否在拖动
	bool isPadding() const;
	// 设置拖动属性
	void setPaddingFrags(PadFlags pf);
	PadFlags getPaddingFrags() const;
	// 选中的item
	QList< DAGraphicsItem* > selectedDAItems() const;
	// 是否空格被按下
	bool isSpacebarPressed() const;
	// 激活一个动作，DAAbstractGraphicsViewAction的内存归view管理,此函数发射viewActionActived
	void setupViewAction(DAAbstractGraphicsViewAction* act);
	// 清除视图action，此操作会把当前维护的视图action清除
	void clearViewAction();
	// 标记一个点,标记完成后
	void markPoint(const QPointF& scenePoint, const QPen& pen = QPen(Qt::blue));
	// 是否marker生效
	bool isEnableMarker() const;
	// 获取当前的markerstyle，如果没生效，返回DAGraphicsViewOverlayMouseMarker::NoMarkerStyle
	DAGraphicsViewOverlayMouseMarker::MarkerStyle getCurrentMarkerStyle() const;
	// 获取marker指针
	DAGraphicsViewOverlayMouseMarker* getMarker() const;
public slots:
	// 放大
	void zoomIn();
	// 缩小
	void zoomOut();
	// 设置最适合视图尺寸
	void zoomFit();
	// 选中所有可选的item
	void selectAll();
	// 取消所有选中
	void clearSelection();
	// 设置标记，此操作基于DAGraphicsViewOverlayMouseMarker
	void setViewMarkerStyle(DAGraphicsViewOverlayMouseMarker::MarkerStyle style);
	// 设置标记是否生效
	void setViewMarkerEnable(bool on);

protected:
	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void keyReleaseEvent(QKeyEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

protected:
	void wheelZoom(QWheelEvent* event);
	// 开始拖动
	void startPad(QMouseEvent* event);
	// 结束拖动
	void endPad();
	// 创建鼠标标记，如果重载了DAGraphicsViewOverlayMouseMarker，需要重载此函数返回自己的MouseMarker
	virtual DAGraphicsViewOverlayMouseMarker* createMarker();

private:
	void init();
	//
	void tryInitViewMarker();
Q_SIGNALS:
	/**
	 * @brief 一个视图动作被激活的信号
	 * @param act
	 */
	void viewActionActived(DA::DAAbstractGraphicsViewAction* act);

	/**
	 * @brief 一个视图动作已经解除激活的信号
	 * @param act
	 */
	void viewActionDeactived(DA::DAAbstractGraphicsViewAction* act);

private:
};
}
#endif  // GGRAPHICSVIEW_H
