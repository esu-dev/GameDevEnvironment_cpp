#include "framework.h"

#include <locale.h>

#include "Direct3D.h"

#include "Texture.h"

Texture::Texture() {}

Texture::Texture(std::string fileName)
{
	Load(fileName);
}

bool Texture::Load(const std::string& fileName)
{
	// �}���`�o�C�g�����񂩂烏�C�h������֕ϊ�
	setlocale(LC_CTYPE, "jpn"); // �n��ݒ����{��ɐݒ�B���{��̓}���`�o�C�g�����ł��邩��ϊ�����K�v������B
	wchar_t wFileName[256]; // wchar_t�̓��C�h�����i���j�R�[�h�j��\�����߂̃f�[�^�^
	size_t ret; // size_t �̓I�u�W�F�N�g�̃o�C�g����\���ł�����x�ɏ\���ɑ傫�������Ȃ������^�B
	mbstowcs_s(&ret, wFileName, fileName.c_str(), 256);

	// WIC(Windows Imaging Component)�摜��ǂݍ���
	auto image = std::make_unique<DirectX::ScratchImage>(); // �C���X�^���X��
	if (FAILED(DirectX::LoadFromWICFile(wFileName, DirectX::WIC_FLAGS_NONE, &m_texMetaData, *image)))
	{
		// ���s
		m_texMetaData = {};
		return false;
	}

	// �~�b�v�}�b�v�̐���
	if (m_texMetaData.mipLevels == 1) // ���̃e�N�X�`���Ƀ~�b�v�}�b�v�����݂��Ȃ��Ȃ��
	{
		auto mipChain = std::make_unique<DirectX::ScratchImage>();
		if (SUCCEEDED(DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, *mipChain)))
		{
			image = std::move(mipChain);
		}
	}

	// ���\�[�X�ƃV�F�[�_�[���\�[�X�r���[���쐬
	if (FAILED(DirectX::CreateShaderResourceView(D3D.m_device.Get(), image->GetImages(), image->GetImageCount(), m_texMetaData, &m_shaderResourceview)))
	{
		// ���s
		m_texMetaData = {};
		return false;
	}

	// ����
	return true;
}