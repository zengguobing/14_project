#ifndef MYTHREAD_H
#define MYTHREAD_H
#include<QMetaType>
#include <QObject>
#include<QDebug>
#include<QThread>
#include<qmutex.h>
#include<FormatConversion.h>
#include<qstandarditemmodel.h>
#include<QDir>
class MyThread : public QObject
{
	Q_OBJECT
	XMLFile* DOC;
public:
	MyThread(QObject* parent = nullptr);
	~MyThread();
public slots:

	

	void Import();
	/** @brief ����sentinel����
	* @param PODFile                      �ڱ�һ�ž�������
	* @param manifest_file                �ڱ�һ��IW���ݲ�Ʒmanifest�ļ���������·����
	* @param subswath                     �Ӵ�ѡ��
	* @param polarization                 ������ʽ
	* @param project_path                 ���̾���·��
	* @param folder                       �������ڵ���
	* @param filename                     �������ļ���������·����
	* @param project_name                 �����ļ���������·����
	* @param model                        treeview�ṹ
	*/
	void import_sentinel(
		QString PODFile,
		QString manifest_file,
		QString subswath,
		QString polarization,
		QString project_path,
		QString folder,
		QString filename,
		QString project_name,
		QStandardItemModel* model
	);
	/*@brief ���������ڱ�����
	* @param original_namelist            ԭʼ�ļ�
	* @param import_namelist              �����ļ�
	* @param subswath                     �Ӵ�
	* @param polarization                 ����
	* @param savepath                     ����·��
	* @param dst_node                     Ŀ��ڵ���
	* @param dst_project                  Ŀ�깤����
	* @param model                        treeviewģ��
	*/
	void import_sentinel_patch(
		vector<QString> original_namelist,
		vector<QString> import_namelist,
		QString subswath,
		QString polarization,
		QString savepath,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);
	/** @brief ����TerraSAR����
	* @param polarization                 ������ʽ
	* @param xml_filename                 xml�ļ���
	* @param project_path                 ���̾���·��
	* @param folder                       �������ڵ���
	* @param filename                     �������ļ���������·����
	* @param project_name                 �����ļ���������·����
	* @param model                        treeview�ṹ
	*/
	void import_TSX(
		QString polarization,
		QString xml_filename,
		QString project_path,
		QString folder,
		QString filename,
		QString project_name,
		QStandardItemModel* model
	);
	/*@brief ��������TerraSAR����
	* @param polarization                ������ʽ
	* @param savepath                    ����·��
	* @param original_file_list          ԭʼ�ļ�
	* @param import_namelist             �����ļ���
	* @param dst_node                    Ŀ��ڵ�
	* @param dst_project                 Ŀ�깤��
	* @param copy                        treeviewģ��
	*/
	void import_TSX_patch(
		QString polarization,
		QString savepath,
		vector<QString> original_file_list,
		vector<QString> import_namelist,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);
	/*@brief ��������COSMOS-SkyMed����
	* @param savepath                    ����·��
	* @param original_file_list          ԭʼ�ļ�
	* @param import_namelist             �����ļ���
	* @param dst_node                    Ŀ��ڵ�
	* @param dst_project                 Ŀ�깤��
	* @param copy                        treeviewģ��
	*/
	void import_CSK_patch(
		QString savepath,
		vector<QString> original_file_list,
		vector<QString> import_namelist,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);
	/*@brief ��������ALOS2����
	* @param savepath                    ����·��
	* @param IMG_file_list               ԭʼ�ļ�(IMG�ļ�)
	* @param LED_file_list2              ԭʼ�ļ�(LED�ļ�)
	* @param import_namelist             �����ļ���
	* @param dst_node                    Ŀ��ڵ�
	* @param dst_project                 Ŀ�깤��
	* @param copy                        treeviewģ��
	*/
	void import_ALOS2_patch(
		QString savepath,
		vector<QString> IMG_file_list,
		vector<QString> LED_file_list,
		vector<QString> import_namelist,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);
	void ShowImage(QString, QString, QString);
	/*@brief ��γ�Ȳü��㷨
	* @param 
	*/
	void Cut(QList<double>, QString, QString, QString, QString, QStandardItemModel*);
	/*@brief ��ѡ�ü��㷨
	* @param
	*/
	void Cut2(
		double h5_left,
		double h5_right,
		double h5_top,
		double h5_bottom,
		QString save_path,
		QString project_name,
		QString node_name,
		QString file,
		QStandardItemModel* model
	);
	void Regis(QList<int>, QString, QString, QString, QString, QStandardItemModel*);
	/*@brief DEM������׼
	* @param masterIndex                ��ͼ�����(��1��ʼ)
	* @param savepath                   ���̱���·��
	* @param project                    ������
	* @param srcNode                    Դ���ݽڵ�
	* @param dstNode                    Ŀ��ڵ�
	* @param model                      ��ģ��
	*/
	void DEMAssistCoregistration(
		int masterIndex,
		QString savepath,
		QString project,
		QString srcNode,
		QString dstNode,
		QStandardItemModel* model
	);
	/*@brief �ڱ�һ������burstƴ�Ӵ���
	* @param savePath                   ����·��
	* @param dstProject                 Ŀ�깤��
	* @param srcNode                    Դ�ڵ�
	* @param dstNode                    Ŀ��ڵ�
	* @param treeModel                  ��ģ��
	*/
	void S1_Deburst(
		QString savePath,
		QString dstProject,
		QString srcNode,
		QString dstNode,
		QStandardItemModel* model
	);
	/*@brief �ڱ�һ�ź�����������׼
	* @param images_number              ͼ�����
	* @param masterIndex                ��ͼ��ţ���1��ʼ��
	* @param savePath                   ����·��
	* @param dstProject                 Ŀ�깤��
	* @param srcNode                    Դ�ڵ�
	* @param dstNode                    Ŀ��ڵ�
	* @param treeModel                  ��ģ��
	* @param b_ESD                      �Ƿ����ESD
	*/
	void S1_TOPS_BackGeocoding(
		int images_number,
		int masterIndex,
		QString savePath,
		QString dstProject,
		QString srcNode,
		QString dstNode,
		QStandardItemModel* model,
		bool b_ESD = true
	);
	/*@brief ��׼��SARͼ���ջȥ�ο���λ��������ƽ�غ͵�����λ��
	* @param masterIndex                ��ͼ����ţ���1��ʼ��
	* @param flat_mode                  ȥƽ��ģʽ��1��ȥƽ����λ��2��ȥ�ο�������λ��
	* @param project_name               ������
	* @param src_node                   ���������ݽڵ���
	* @param dst_node                   �������ڵ���
	* @param model                      ���̹�����ģ��
	*/
	void SLC_deramp(
		int masterIndex,
		int flat_mode,
		QString project_name,
		QString src_node,
		QString dst_node,
		QStandardItemModel* model
	);
	/*@brief ���ƻ��߷ֲ�������С���߼�������Ͼ���
	* @param masterIndex               ��ͼ����ţ���1��ʼ��
	* @param project_name              ������
	* @param src_node                  ���������ݽڵ�
	* @param model                     ���̹�����ģ��
	*/
	void Baseline_Formation(
		int masterIndex,
		QString project_name,
		QString src_node,
		QStandardItemModel* model
	);
	/*@brief С���߼�ʱ�����з���
	* @param temporal_thresh_low        ʱ����ߵ���ֵ��day��
	* @param temporal_thresh            ʱ����߸���ֵ��day��
	* @param spatial_thresh             �ռ������ֵ��m��
	* @param multilook_rg               ��������ӱ���
	* @param multilook_az               ��λ����ӱ���
	* @param unwrap_method              ���������1��Delaunay_MCF��2��SNAPHU��3��MCF��
	* @param Goldstein_alpha            Goldstein�˲�ǿ��
	* @param coherence_thresh           �������ֵ
	* @param temporal_coherence_thresh  ģ�����ϵ����ֵ
	* @param refinement_coh_thresh      �����������ȥƽ���Ƶ�ɸѡ���ϵ����ֵ
	* @param refinemen_def_thresh       �����������ȥƽ���Ƶ�ɸѡ�α�������ֵ��m/year��
	* @param project                    ������
	* @param srcNode                    ���ݽڵ���
	* @param dstNode                    Ŀ��ڵ�
	* @param model                      ���̹�����ģ��
	*/
	void SBAS_time_series(
		double temporal_thresh_low,
		double temporal_thresh,
		double spatial_thresh,
		int multilook_rg,
		int multilook_az,
		int unwrap_method,
		double alpha,
		double coherence_thresh,
		double temporal_coherence_thresh,
		double refinement_coh_thresh,
		double refinemen_def_thresh,
		QString project,
		QString srcNode,
		QString dstNode,
		QStandardItemModel* model
	);
	/*@brief SBASʱ�����з����ο�����ѡ
	* @param project            Ŀ�깤��
	* @param srcNode            Ŀ�����ݽڵ�
	* @param ref_row            �ο�������
	* @param ref_col            �ο�������
	* @param GCPs               ��ȥƽ�ο���
	* @param model              ���̹�����ģ��
	*/
	void SBAS_reference_reselection(
		QString project,
		QString srcNode,
		int ref_row,
		int ref_col,
		QList<QPoint> GCPs,
		QStandardItemModel* model
	);
	/*@brief �������
	* @param type              ����������1�������Ʒ��2��SAR��ͼ��
	* @param multi_rg          ��������ӱ���������SARͼ�������룩
	* @param multi_az          ��λ����ӱ���������SARͼ�������룩
	* @param project           ������
	* @param srcNode           Դ�ڵ���
	* @param dstNode           Ŀ��ڵ���
	* @param model             ���̹�����ģ��
	*/
	void Geocoding(
		int type,
		int multi_rg,
		int multi_az,
		QString project,
		QString srcNode,
		QString dstNode,
		QStandardItemModel* model
	);
	/*@brief �ڱ�һ�Ÿ�����λ�Ӵ�ƴ��
	* @param index1            �Ӵ�1������λͼ����ţ�1-based��
	* @param index2            �Ӵ�2������λͼ�����
	* @param index3            �Ӵ�3������λͼ�����
	* @param project_name      �����ļ���
	* @param srcNode1          �Ӵ�1������λ���ݽڵ�
	* @param srcNode2          �Ӵ�2������λ���ݽڵ�
	* @param srcNode3          �Ӵ�3������λ���ݽڵ�
	* @param dstNode           Ŀ��ڵ�
	* @param model             ���̹�����ģ��
	*/
	void S1_swath_merge(
		int index1,
		int index2,
		int index3,
		QString project_name,
		QString srcNode1,
		QString srcNode2,
		QString srcNode3,
		QString dstNode,
		QStandardItemModel* model
	);
	/*@brief �ڱ�һ������frame���Ӹ�ͼ��ƴ��
	* @param index1            �Ӵ�1������λͼ����ţ�1-based��
	* @param index2            �Ӵ�2������λͼ�����
	* @param project_name      �����ļ���
	* @param srcNode1          �Ӵ�1������λ���ݽڵ�
	* @param srcNode2          �Ӵ�2������λ���ݽڵ�
	* @param dstNode           Ŀ��ڵ�
	* @param model             ���̹�����ģ��
	*/
	void S1_frame_merge(
		int index1,
		int index2,
		QString project_name,
		QString srcNode1,
		QString srcNode2,
		QString dstNode,
		QStandardItemModel* model
	);

	/*@brief ���뵥����������
	* @param savepath                     ����·��
	* @param original_namelist            ԭʼ�����ļ�������
	* @param import_namelist              ������ļ�������
	* @param dst_node                     ���빤�̽ڵ���
	* @param dst_project                  ���빤����
	* @param model                        ������ģ��
	*/
	void import_RepeatPass(
		QString savepath,
		vector<QString> original_namelist,
		vector<QString> import_namelist,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);

	/*@brief ���뵥��˫������
	* @param savepath                     ����·��
	* @param master_file                  ��ͼ�ļ�
	* @param slave_file                   ��ͼ�ļ�
	* @param dst_node                     ���빤�̽ڵ���
	* @param dst_project                  ���빤����
	* @param model                        ������ģ��
	*/
	void import_SingleTransDoubleRecv(
		QString savepath,
		QString master_file,
		QString slave_file,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);

	/*@brief ����ƹ��ģʽ����
	* @param import_file_list             �����ļ�list
	* @param savepath                     ����·��
	* @param dst_node                     ���빤�̽ڵ���
	* @param dst_project                  ���빤����
	* @param model                        ������ģ��
	*/
	void import_PingPong(
		vector<QString> import_file_list,
		QString savepath,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);

	/*@brief ����˫Ƶƹ��ģʽ����
	* @param import_file_list             �����ļ�list
	* @param savepath                     ����·��
	* @param dst_node                     ���빤�̽ڵ���
	* @param dst_project                  ���빤����
	* @param model                        ������ģ��
	*/
	void import_DualFreqPingPong(
		vector<QString> import_file_list,
		QString savepath,
		QString dst_node,
		QString dst_project,
		QStandardItemModel* model
	);

	/*@brief ������λ����
	* @param master_index                 ��ͼ�����кţ�1-based��
	* @param multilook_rg                 ��������ӱ���
	* @param multilook_az                 ��λ����ӱ���
	* @param savepath                     ����·��
	* @param src_project                  ���������ļ�
	* @param src_node                     ���������ݽڵ�
	* @param dst_node                     Ŀ�����ݽڵ�
	* @param model                        ������ģ��
	*/
	void interferogram_generation_14(
		int master_index,
		int multilook_rg,
		int multilook_az,
		QString savepath,
		QString src_project,
		QString src_node,
		QString dst_node,
		QStandardItemModel* model
	);

	void Baseline_Estimate(int index, QString project_name, QString dst_node, const QStandardItemModel*);
	void Interferometric(bool isdeflat, bool istopo_removal, bool iscoherence, int master_index, int win_r, int win_c,
		int multilook_row, int multilook_col, QString, QString, QString, QString, QStandardItemModel*);
	void Denoise(QList<int>, double, QString, QString, QString, QString, QStandardItemModel*);
	void QUnwrap(int, double,  QString, QString, QString, QString, QStandardItemModel*);
	void QDem(int, int, QString, QString, QString, QString, QStandardItemModel*);
	void StopProcess();
signals:
	void updateProcess(int, QString);
	void endProcess();
	/*���������Ϣ*/
	void errorProcess(QString error_msg);
	void sendModel(QStandardItemModel*);
	void sendBL(QList<double>temporal_baseline, QList<double>spatial_baseline, int index);
private:
	QMutex lock;
	bool stop_flag;
	int Registration_copy(
		vector<string>& SAR_images,
		vector<string>& SAR_images_out,
		Mat& offset_row_out,
		Mat& offset_col_out,
		int Master_index, 
		int interp_times,
		int blocksize
	);
	int complex_coherence(
		const ComplexMat& master_image,
		const ComplexMat& slave_image,
		int est_wndsize_rg,
		int est_wndsize_az,
		Mat& coherence);
	int change_suffix(const char* input, QString output_str, QString old_suffix, QString new_suffix);
};

#endif // MYTHREAD_H