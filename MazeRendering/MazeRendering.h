//[MazeRendering.h]

#pragma once

namespace PSEUDO_3D_MAZE
{
	class IMazeData;
	class RenderProp;

	//���H�`�揈���ŕK�v�ȕ`�揈�������ۂɎ��{����ҁD
	//�\���p�摜�o�b�t�@�ɕ`����s����ځD
	class IRenderTgt
	{
	public:
		virtual ~IRenderTgt() = default;

	public:
		//���H��`�悷��G�̃T�C�Y[pixel]��Ԃ�
		virtual int ImageWidth() const = 0;
		virtual int ImageHeight() const = 0;

		//�G�b�W�̈ꕔ�Ƃ��ďc����`�悷��D
		//�G�b�W���ǁC���邢�͂���ɏ�������̂ł���Ƃ��ɂ����`�悷�邽�߂ɌĂ΂��̂ŁC
		//�`��摜��́C��1pixel�̐��̈� (x,y0)-(x,y1) ��`�悷��D�i�����l�̑召�֌W�� y0 <= y1 �ł���j
		//
		//EdgeAttr : �`��ΏۃG�b�W�̑����l
		//Depth : �`��Ώۈʒu�̃J��������̉��s��
		//Dist : �`��Ώۈʒu�̃J��������̋���
		//LRPosRateOnEdge : �`��ΏۃG�b�W��^���ʂɂ݂��Ƃ��́C�`��c���̃G�b�W��ł̉������ʒu�D(���[0.0�`�E�[1.0)
		virtual void DrawVerticalLine( int x, int y0, int y1, int EdgeAttr, double Depth, double Dist, double LRPosRateOnEdge ) = 0;
	};


	//�s���z�[���J�������f���ɂ��3D���H�̕`��
	//
	//��Ԃ̍��W�n�ɂ��āF
	//	���_�F�}�X(0,0)�̖k���[
	//	���W�̌����F���ɂ����ق�PosX�������C��ɂ����ق�PosY�������D
	//	���W�̎ړx�F1�}�X�̕ӂ̒�����1.0�Ƃ���D
	//	���Ȃ킿�C�}�X(x,y)�̒��S�ʒu = (x+0.5,y+0.5) �ƂȂ�D
	//
	//[Args]
	//	rMazeData :
	//		���H�f�[�^�D�`�揈�����K�v�Ƃ���f�[�^��񋟂���D
	//	RenderProp :
	//		�����_�����O�p�v���p�e�B�D�`���̐ݒ�D
	//	rRenderTgt :
	//		�摜�o�b�t�@�ւ̕`�揈���ҁD
	void RenderMazeEdge(
		const IMazeData &rMazeData,	//���H�f�[�^
		const RenderProp &rProp,	//�����_�����O�p�v���p�e�B
		IRenderTgt &rRenderTgt	//�`�揈����
	);

}

//[EOF]