#pragma once
#include <string>

/// <summary>Exe�̂���f�B���N�g���̃p�X������D</summary>
/// <returns>UTF16�D�����Ƀp�X��؂�L�� (/) ���t����������</returns>
const std::wstring &ExeDirU16();

/// <summary>Exe�̂���f�B���N�g���̃p�X������D</summary>
/// <returns>UTF8�D�����Ƀp�X��؂�L�� (/) ���t����������</returns>
const std::string &ExeDirU8();

//---

/// <summary>�摜�f�[�^�z�u�f�B���N�g���̃p�X������D</summary>
/// <returns>UTF16�D�����Ƀp�X��؂�L�� (/) ���t����������</returns>
std::wstring ImgDirU16();


//
///// <summary>
///// �f�[�^�t�@�C��
/////   * �}�b�v�f�[�^�t�@�C��
/////   * �C�x���g��`�t�@�C��
/////   * �C�x���g�X�N���v�g�t�@�C��
///// ��z�u���ׂ��f�B���N�g���̃p�X
///// </summary>
///// <returns>�����Ƀp�X��؂�L�� (/) ���t����������<</returns>
//std::string DataFileDirU8();
//
///// <summary>�}�b�v�f�[�^�t�@�C���̃p�X</summary>
///// <returns>UTF8</returns>
//std::string MazeMapFilePathU8();
//
///// <summary>�C�x���g��`�t�@�C���̃p�X</summary>
///// <returns>UTF8</returns>
//std::string EventDefinitionFilePathU8();
//
///// <summary>�C�x���g�X�N���v�g�t�@�C���̃p�X</summary>
///// <returns>UTF8</returns>
//std::string EventScriptFilePathU8( int EventID );
//
///// <summary>�Q�[���X�g�[���[�����p�C�x���g�X�N���v�g�t�@�C���̃p�X</summary>
///// <returns>UTF8</returns>
//inline std::string GameStroyDescEventScriptFilePathU8(){	return EventScriptFilePathU8(998);	}
//
///// <summary>�Q�[���N���A���p�C�x���g�X�N���v�g�t�@�C���̃p�X</summary>
///// <returns>UTF8</returns>
//inline std::string GameClearEventScriptFilePathU8(){	return EventScriptFilePathU8(999);	}
//
///// <summary>�Z�[�u�t�@�C���z�u�f�B���N�g���̃p�X</summary>
///// <returns>�����Ƀp�X��؂�L�� (/) ���t����������<</returns>
//std::string GameSaveFileDirU8();
//
///// <summary>�Z�[�u�t�@�C���̃p�X</summary>
///// <returns>UTF8</returns>
//std::string GameSaveFilePathU8();
//
///// <summary>�T�E���h�t�@�C���Q�z�u�f�B���N�g���̃p�X</summary>
///// <returns>�����Ƀp�X��؂�L�� (/) ���t����������<</returns>
//std::wstring SoundDirU16();
//
///// <summary>BGM,SE�̃t�@�C���Q�̖��̂�������Ă���t�@�C���ւ̃p�X</summary>
///// <returns>UTF8</returns>
//std::string SoundEnumerationFileOathU8();

