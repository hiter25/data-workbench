﻿#ifndef DAWORKFLOWEDITWIDGET_H
#define DAWORKFLOWEDITWIDGET_H
#include <QWidget>
#include <QImage>
#include "DAGuiAPI.h"
#include "DAGraphicsStandardTextItem.h"
#include "DAWorkFlowGraphicsView.h"
#include "DAWorkFlowGraphicsScene.h"
#include "DAWorkFlow.h"
#include "DAAbstractGraphicsSceneAction.h"
namespace Ui
{
class DAWorkFlowEditWidget;
}
class QUndoStack;

namespace DA
{
class DAGraphicsItem;
class DADataWorkFlow;
class DAWorkFlowGraphicsView;
class DAAbstractNodeGraphicsItem;
/**
 * @brief 工作流绘图窗口
 *
 * 工作流窗口管理一个scene，后续可存在多个view
 */
class DAGUI_API DAWorkFlowEditWidget : public QWidget
{
	Q_OBJECT

public:
	explicit DAWorkFlowEditWidget(QWidget* parent = nullptr);
	~DAWorkFlowEditWidget();
	// 获取工厂
	DAWorkFlow* getWorkflow() const;
	// 设置工作流
	void setWorkFlow(DAWorkFlow* w);
	// 获取工作流操作视图
	DAWorkFlowGraphicsView* getWorkFlowGraphicsView() const;
	// 获取GraphicsScene
	DAWorkFlowGraphicsScene* getWorkFlowGraphicsScene() const;
	// 激活UndoStack
	void setUndoStackActive();
	// 设置显示grid
	void setEnableShowGrid(bool on);
	// 获取QUndoStack
	QUndoStack* getUndoStack();
	// 运行工作流
	void runWorkFlow();
	// 设置鼠标动作
	void setPreDefineSceneAction(DAWorkFlowGraphicsScene::SceneActionFlag mf);
	// 设置文本字体 -- 此参数设置决定创建文本框时的字体和颜色
	QFont getDefaultTextFont() const;
	void setDefaultTextFont(const QFont& f);
	// 设置文本颜色 -- 此参数设置决定创建文本框时的字体和颜色
	QColor getDefaultTextColor() const;
	void setDefaultTextColor(const QColor& c);
	// 添加一个图片item
	DAGraphicsPixmapItem* addPixmapItem_(const QImage& img);
	// 获取当前view视图下的scene中心
	QPointF getViewCenterMapToScene() const;
	// 把item移动到屏幕中心
	void moveItemToViewSceneCenter(QGraphicsItem* item);
	// 计算item所包含的范围，这个范围存入xml中，以便让scene第一时间知道总体范围
	static QRectF calcAllItemsSceneRange(const QList< QGraphicsItem* >& its);
	//
	static QList< QGraphicsItem* > cast(const QList< DAGraphicsItem* >& its);
public slots:
	// 添加一个背景图
	void addBackgroundPixmap(const QString& pixmapPath);
	// 锁定背景图
	void setBackgroundPixmapLock(bool on);
	// 文字加粗
	void setSelectTextToBold(bool on);
	// 文字斜体
	void setSelectTextToItalic(bool on);
	// 设置文字颜色
	void setSelectTextColor(const QColor& color);
	// 设置字体样式
	void setSelectTextFamily(const QString& family);
	// 设置字体大小
	void setSelectTextPointSize(const int size);
	// 设置当前选择的文本item的字体
	void setSelectTextItemFont(const QFont& f);
	// 设置当前选中的图元的背景
	void setSelectShapeBackgroundBrush(const QBrush& b);
	// 设置当前选中的图元的背景
	void setSelectShapeBorderPen(const QPen& v);
	// 全选
	void selectAll();
	// 取消选中
	void clearSelection();
	// 复制当前选中的items
	void copySelectItems();
	// 复制当前选中的items
	void cutSelectItems();
	// 粘贴到视图中心
	void pasteToViewCenter();
	// 移除选中的条目
	void removeSelectItems();
	// 执行取消动作
	void cancel();
signals:

	/**
	 * @brief 选中了某个节点的设置窗口
	 * @param w
	 */
	void selectNodeItemChanged(DA::DAAbstractNodeGraphicsItem* i);

	/**
	 * @brief 场景动作激活
	 * @param scAction 场景动作指针
	 */
	void sceneActionActived(DA::DAAbstractGraphicsSceneAction* scAction);

	/**
	 * @brief 场景动作结束
	 * @param scAction 场景动作指针
	 */
	void sceneActionDeactived(DA::DAAbstractGraphicsSceneAction* scAction);
	/**
	 * @brief 开始执行，exec函数调用后会触发此信号
	 */
	void startExecute();
	/**
	 * @brief 执行到某个节点发射的信号
	 * @param n
	 */
	void nodeExecuteFinished(DAAbstractNode::SharedPointer n, bool state);

	/**
	 * @brief 工作流执行完毕信号
	 * @param success 成功全部执行完成为true
	 */
	void finished(bool success);

private:
	// 获取选中的文本
	QList< DAGraphicsStandardTextItem* > getSelectStandardTextItems();
	QList< DAGraphicsTextItem* > getSelectTextItems();
	// 获取选中的基本图元
	QList< DAGraphicsItem* > getSelectDAItems();
	void createScene();
	// 设置item的选中状态
	void setSelectionState(const QList< QGraphicsItem* >& items, bool isSelect);

private:
	Ui::DAWorkFlowEditWidget* ui;
	DAWorkFlowGraphicsScene* mScene { nullptr };
};
}  // end of DA
#endif  // DAWORKFLOWEDITWIDGET_H
