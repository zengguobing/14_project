/*-------------------------------------------------------------------*/
/*          �����ṹ���ļ�������ģ�崦��ʱ���θ��������             */
/*-------------------------------------------------------------------*/
#pragma once
#define MAX_FILE_LENGTH 1024
/**
* DEM����ģ������ṹ��
*/
struct template_DEM_para
{
	/*������*/
	char project_name[MAX_FILE_LENGTH];
	/*����Ŀ¼*/
	char working_directory[MAX_FILE_LENGTH];
	/*��ͼ���ļ�*/
	char master_image_file[MAX_FILE_LENGTH];
	/*��ͼ���ļ�*/
	char slave_image_file[MAX_FILE_LENGTH];


	/*-----------------*/
	/*   ͼ�������  */
	/*-----------------*/

	/*��������*/
	char sensor[256];


	/*-----------------*/
	/*   AOI�ü�����   */
	/*-----------------*/

	/*AOI���ľ���*/
	double AOI_center_lon;
	/*AOI����γ��*/
	double AOI_center_lat;
	/*AOI�߶�(m)*/
	double AOI_height;
	/*AOI���(m)*/
	double AOI_width;


	/*-----------------*/
	/*  ͼ����׼����   */
	/*-----------------*/

	/*��ֵ����*/
	int coregis_interp_times;
	/*�ӿ��С*/
	int coregis_blocksize;

	/*---------------------*/
	/*  ������λ���ɲ���   */
	/*---------------------*/

	/*��������ӱ���*/
	int ifg_multilook_rg;
	/*��λ����ӱ���*/
	int ifg_multilook_az;
	/*�Ƿ�������ϵ��*/
	bool ifg_b_coh;
	/*���ϵ�����Ʒ�λ�򴰿ڴ�С*/
	int ifg_coh_wndsize_az;
	/*���ϵ�����ƾ����򴰿ڴ�С*/
	int ifg_coh_wndsize_rg;

	/*-----------------*/
	/*  ��λ�˲�����   */
	/*-----------------*/

	/*�˲�����(1��б������Ӧ��2��Goldstein��3�����ѧϰ)*/
	int denoise_method;
	/*б������ӦԤ�˲����ڴ�С*/
	int slope_pre_size;
	/*б������Ӧ�˲����ڴ�С*/
	int slope_size;
	/*Goldstein�˲��̶ȣ�0-1֮�䣩*/
	double Goldstein_alpha;
	/*Goldstein�˲�FFT���ڴ�С*/
	int Goldstein_FFT_wdnsize;
	/*Goldstein�˲�FFT���㴰�ڴ�С*/
	int Goldstein_pad;

	/*-----------------*/
	/*  ��λ�������   */
	/*-----------------*/

	/*���������1������ͼ����2����С����������3��SNAPHU��4���ۺϽ������*/
	int unwrap_method;
	/*�ۺϽ�������ϵ����ֵ(0-1֮��)*/
	double unwrap_coh_thresh;

	/*-----------------*/
    /*  �̷߳��ݲ���   */
    /*-----------------*/

	/*ţ�ٵ�������������*/
	int iter_times;


	template_DEM_para()
	{
		project_name[0] = 0;
		working_directory[0] = 0;
		master_image_file[0] = 0;
		slave_image_file[0] = 0;
		sensor[0] = 0;

		AOI_center_lat = 0.0;
		AOI_center_lon = 0.0;
		AOI_width = 5000;
		AOI_height = 5000;

		coregis_interp_times = 8;
		coregis_blocksize = 256;

		ifg_b_coh = true;
		ifg_coh_wndsize_az = 3;
		ifg_coh_wndsize_rg = 3;
		ifg_multilook_az = 1;
		ifg_multilook_rg = 1;

		denoise_method = 2;
		slope_pre_size = 11;
		slope_size = 11;
		Goldstein_alpha = 0.9;
		Goldstein_FFT_wdnsize = 64;
		Goldstein_pad = 8;

		unwrap_method = 2;
		unwrap_coh_thresh = 0.7;

		iter_times = 20;
	}

};

/*DEMģ�崦���ӽ��̻ش��ļ������ṹ�壨���ڸ��½��湤������xml�����ļ���*/
struct template_DEM_para_back
{
	/*--------------*/
	/*   ��������   */
	/*--------------*/

	/*�������ݽڵ���*/
	char import_node_name[MAX_FILE_LENGTH];
	/*������ͼ���ļ���������·����.h5��׺��*/
	char import_master_name[MAX_FILE_LENGTH];
	/*���븨ͼ���ļ���������·����.h5��׺��*/
	char import_slave_name[MAX_FILE_LENGTH];
	/*������ͼ���ļ�*/
	char import_master_file[MAX_FILE_LENGTH];
	/*���븨ͼ���ļ�*/
	char import_slave_file[MAX_FILE_LENGTH];

	/*--------------*/
	/*   �ü�AOI    */
	/*--------------*/

	/*�ü�AOI���ݽڵ���*/
	char AOI_node_name[MAX_FILE_LENGTH];
	/*�ü���ͼ���ļ���������·����.h5��׺��*/
	char AOI_master_name[MAX_FILE_LENGTH];
	/*�ü���ͼ���ļ���������·����.h5��׺��*/
	char AOI_slave_name[MAX_FILE_LENGTH];
	/*�ü���ͼ���ļ�*/
	char AOI_master_file[MAX_FILE_LENGTH];
	/*�ü���ͼ���ļ�*/
	char AOI_slave_file[MAX_FILE_LENGTH];


	/*--------------*/
	/*     ��׼     */
	/*--------------*/

	/*��׼���ݽڵ���*/
	char regis_node_name[MAX_FILE_LENGTH];
	/*��׼��ͼ���ļ���������·����.h5��׺��*/
	char regis_master_name[MAX_FILE_LENGTH];
	/*��׼��ͼ���ļ���������·����.h5��׺��*/
	char regis_slave_name[MAX_FILE_LENGTH];
	/*��׼��ͼ���ļ�*/
	char regis_master_file[MAX_FILE_LENGTH];
	/*��׼��ͼ���ļ�*/
	char regis_slave_file[MAX_FILE_LENGTH];


	/*--------------*/
	/*     ����     */
	/*--------------*/

	/*������λ�������ݽڵ���*/
	char ifg_node_name[MAX_FILE_LENGTH];
	/*������λ�ļ���������·����.h5��׺��*/
	char ifg_name[MAX_FILE_LENGTH];
	/*������λ�ļ�*/
	char ifg_file[MAX_FILE_LENGTH];
	/*���ϵ���ļ���������·����.h5��׺��*/
	char ifg_coherence_name[MAX_FILE_LENGTH];

	/*--------------*/
	/*     �˲�     */
	/*--------------*/

	/*�˲����ݽڵ���*/
	char denoise_node_name[MAX_FILE_LENGTH];
	/*�˲���λ�ļ���������·����.h5��׺��*/
	char denoise_ifg_name[MAX_FILE_LENGTH];
	/*�˲���λ�ļ�*/
	char denoise_ifg_file[MAX_FILE_LENGTH];

	/*--------------*/
	/*     ���     */
	/*--------------*/

	/*������ݽڵ���*/
	char unwrap_node_name[MAX_FILE_LENGTH];
	/*�����λ�ļ���������·����.h5��׺��*/
	char unwrap_ifg_name[MAX_FILE_LENGTH];
	/*�����λ�ļ�*/
	char unwrap_ifg_file[MAX_FILE_LENGTH];


	/*--------------*/
	/*     �߳�     */
	/*--------------*/

	/*�̷߳������ݽڵ���*/
	char dem_node_name[MAX_FILE_LENGTH];
	/*�߳��ļ���������·����.h5��׺��*/
	char dem_name[MAX_FILE_LENGTH];
	/*�߳��ļ�*/
	char dem_file[MAX_FILE_LENGTH];


	template_DEM_para_back()
	{
		import_node_name[0] = 0; import_master_name[0] = 0; import_master_file[0] = 0; import_slave_name[0] = 0; import_slave_file[0] = 0;
		AOI_node_name[0] = 0; AOI_master_name[0] = 0; AOI_master_file[0] = 0; AOI_slave_name[0] = 0; AOI_slave_file[0] = 0;
		regis_node_name[0] = 0; regis_master_name[0] = 0; regis_master_file[0] = 0; regis_slave_name[0] = 0; regis_slave_file[0] = 0;
		ifg_node_name[0] = 0; ifg_name[0] = 0; ifg_file[0] = 0; ifg_coherence_name[0] = 0;
		denoise_node_name[0] = 0; denoise_ifg_name[0] = 0; denoise_ifg_file[0] = 0;
		unwrap_node_name[0] = 0; unwrap_ifg_name[0] = 0; unwrap_ifg_file[0] = 0;
		dem_node_name[0] = 0; dem_name[0] = 0; dem_file[0] = 0;
	}
};