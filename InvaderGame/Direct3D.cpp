// �Q�l�@https://gamesgard.com/directx11_lesson02/

#include "framework.h"

#include "Direct3D.h"
#include "Shader.h"

#include "GameEngine.h"
#include "GameSystem.h"


Direct3D::Direct3D() :
	_textureShader(new Shader(L"Shader/SpriteShader.hlsl", "VS", "PS")),
	_colorShader(new Shader(L"Shader/SpriteShader.hlsl", "VS", "PS_Color")) {}

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
	//=====================================================
	// �t�@�N�g���[�쐬(�r�f�I �O���t�B�b�N�̐ݒ�̗񋓂�w��Ɏg�p�����I�u�W�F�N�g)
	//=====================================================
	// ComPtr��Generics�Ƃ������Ƃ��H
	ComPtr<IDXGIFactory> factory;

	// Create���悤�Ƃ��Č��ʂ�����������i���ɂ���j�������Ȃ��B
	// COM�C���^�[�t�F�[�X�֘A�̊֐��̖߂�l�ł�HRESULT�^�����p����邱�Ƃ������B
	// FAILED�}�N����HRESULT�^�̖߂�l�𔻒肷�邱�Ƃ��ł���B
	// ���\�b�h�͐����������A���ʂ����������Ƃ���s_FALSE���Ԃ����B
	// ������󂯎��ƁAFAILED�}�N����true��Ԃ��B
	// HRESULT�^��long�^�̐��l
	// CreateDXGIFactory1(); DXGI�t�@�N�g���̐���
	// IID_PPV_ARGS�}�N�� �C���^�[�t�F�[�X�|�C���^���擾���邽�߂Ɏg�p�����B�Q�̕ϐ��ƂȂ�
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
	{
		return false;
	}

	//=====================================================
	//�f�o�C�X����(��Ƀ��\�[�X�쐬���Ɏg�p����I�u�W�F�N�g)
	//=====================================================
	UINT creationFlags = 0;

#ifdef _DEBUG
	// DEBUG�r���h����Direct3D�̃f�o�b�O��L���ɂ���
	// (�������d�����ׂ����G���[���킩��)
	// D3D11_CREATE_DEVICE_DEBUG:enum�̒��g�̂ЂƂB
	// �f�o�b�O���C���[���T�|�[�g����f�o�C�X���쐬����t���O�𗧂Ă���B
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// FeatureLevel��z��ŕێ����Ă��������
	// FeatureLevel�̓h���C�o�̋@�\���x�����w���Ă���B
	// �h���C�o��OS����Ӌ@���PC���̃n�[�h�����n�����A���삳����������ʂ����B
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,	// Direct3D 11.1  ShaderModel 5
		D3D_FEATURE_LEVEL_11_0,	// Direct3D 11    ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,	// Direct3D 10.1  ShaderModel 4
		D3D_FEATURE_LEVEL_10_0,	// Direct3D 10.0  ShaderModel 4
		D3D_FEATURE_LEVEL_9_3,	// Direct3D 9.3   ShaderModel 3
		D3D_FEATURE_LEVEL_9_2,	// Direct3D 9.2   ShaderModel 3
		D3D_FEATURE_LEVEL_9_1,	// Direct3D 9.1   ShaderModel 3
	};

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g���쐬
	D3D_FEATURE_LEVEL futureLevel;
	if (FAILED(D3D11CreateDevice(
			nullptr, // IDXGIAdapter*
			D3D_DRIVER_TYPE_HARDWARE, // D3D_DRIVER_TYPE
			nullptr, // HMODULE
			creationFlags, // UINT
			featureLevels, // D3D_FEATURE_LEVEL*
			_countof(featureLevels), // UINT
			D3D11_SDK_VERSION, // UINT
			&m_device, // ID3D11Device**
			&futureLevel, // D3D_FEATURE_LEVEL*
			&m_deviceContext // ID3D11DeviceContext**
		)))
	{
		return false;
	}

	//=====================================================
	// �X���b�v�`�F�C���쐬(�t�����g�o�b�t�@�ɕ\���\�ȃo�b�N�o�b�t�@��������)
	//=====================================================
	// �o�b�N�o�b�t�@�Ƃ̓f�B�X�v���C�ɕ\������Ȃ����z�I�ȉ�ʂ̂���
	DXGI_SWAP_CHAIN_DESC scDesc = {};		// �X���b�v�`�F�[���̐ݒ�f�[�^
	scDesc.BufferDesc.Width = width;						// ��ʂ̕�
	scDesc.BufferDesc.Height = height;						// ��ʂ̍���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�t�@�̌`���B�A���t�@���܂ރ`���l�������� 8 �r�b�g���T�|�[�g���� 4 �R���|�[�l���g�� 32 �r�b�g�����Ȃ����K�������`���B��ʓI��256�����番����₷��
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // �X�L�������C���i�������j�̎w��
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // �E�B���h�E�`�掞�̃X�P�[�����O�̎w��
	scDesc.BufferDesc.RefreshRate.Numerator = 0; // ���t���b�V�����[�g�̕��q
	scDesc.BufferDesc.RefreshRate.Denominator = 1; // ���t���b�V�����[�g�̕���
	scDesc.SampleDesc.Count = 1;							// �P�s�N�Z���̐F�����߂�T���v�����O�� �}���`�T���v�����O�͎g�p���Ȃ��B�A���`�G�C���A�X�������ɁA���ԐF�⊮�̊�F�������̃T���v�������ɂ��邩�̐ݒ�B�d���B
	scDesc.SampleDesc.Quality = 0;							// �}���`�T���v�����O�̕i���B�g�p���Ȃ�
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�t�@�̎g�p���@
	scDesc.BufferCount = 2;									// �X���b�v�`�F�C���̃o�b�t�@�̐�
	scDesc.OutputWindow = hWnd; // �o�͐�E�B���h�E�n���h��
	scDesc.Windowed = TRUE;									// �E�B���h�E���[�h
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // �X���b�v�G�t�F�N�g�̐ݒ�B�f�B�X�v���C�\����̃o�b�N�o�b�t�@�̏�Ԃɂ��āH�������߂�Discard
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �X���b�v�`�F�C���̐ݒ�t���O�̐ݒ�B�E�B���h�E���[�h����t���X�N���[���ɐ؂�ւ���ƁA�A�v���P�[�V�����E�B���h�E�̃T�C�Y�ɍ��킹�ĉ𑜓x���ύX�����ݒ�
	
	// �X���b�v�`�F�C���̍쐬
	// ComPtr���A���[���Z�q���I�[�o�[���[�h���Ă��邩��
	if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
	{
		return false;
	}

	// �X���b�v�`�F�C������o�b�N�o�b�t�@���\�[�X�擾
	ComPtr<ID3D11Texture2D> pBackBuffer;
	if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
	{
		return false;
	}

	// �o�b�N�o�b�t�@���\�[�X�p��RTV���쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = scDesc.BufferDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // ���\�[�X�� 2D �e�N�X�`���Ƃ��ăA�N�Z�X����܂��B
	if (FAILED(m_device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &m_backBufferView)))
	{
		return false;
	}

	//=====================================================
	// �f�o�C�X�R���e�L�X�g�ɕ`��Ɋւ���ݒ���s���Ă���
	//=====================================================

	// �o�b�N�o�b�t�@��RT�Ƃ��ăZ�b�g
	// ������GetAddressOf()���g���̂́AComPtr�ł�&���I�[�o�[���[�h����Ă��邩��
	m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
	m_deviceContext->RSSetViewports(1, &vp);

	//=====================================================
	// �V�F�[�_�[�̍쐬
	//=====================================================
	_textureShader->CreateShader(*m_device.Get());
	_colorShader->CreateShader(*m_device.Get());

	return true;
}

void Direct3D::ChangeMode_2D()
{
	// �l�p�`�p���_�o�b�t�@���쐬
	if (m_vbSquare == nullptr)
	{
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// �f�o�C�X�Ƀo�C���h����Ƃ��̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�Ȃ�)
		vbDesc.ByteWidth = sizeof(VertexType2D) * 4;	// �쐬����o�b�t�@�̃o�C�g�T�C�Y
		vbDesc.MiscFlags = 0;							// ���̑��̃t���O
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;				// �쐬����o�b�t�@�̎g�p�@
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		m_device->CreateBuffer(&vbDesc, nullptr, &m_vbSquare);
	}

	// ���_�o�b�t�@��`��Ŏg����悤�ɃZ�b�g����
	UINT stride = sizeof(VertexType2D); // �P���_�̃o�C�g�T�C�Y��������
	UINT offset = 0;
	D3D.m_deviceContext->IASetVertexBuffers(0, 1, m_vbSquare.GetAddressOf(), &stride, &offset); // �f�o�C�X�R���e�L�X�g�ɒ��_�o�b�t�@���Z�b�g

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g
	D3D.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �萔���̒ǉ�
	D3D11_BUFFER_DESC bufferDesk = {};
	bufferDesk.Usage = D3D11_USAGE_DEFAULT;
	bufferDesk.ByteWidth = sizeof(ConstantBuffer);
	bufferDesk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesk.CPUAccessFlags = 0;

	// �萔�o�b�t�@�̐ݒ�
	if (FAILED(m_device->CreateBuffer(&bufferDesk, nullptr, _constantBuffer.GetAddressOf())))
	{
		MessageBox(NULL, L"�萔�o�b�t�@���쐬�ł��܂���ł����B", L"�G���[�E�B���h�E", MB_OK | MB_ICONERROR);
		return;
	}


	// �J���[�萔�o�b�t�@�̍쐬
	// �萔���̒ǉ�
	D3D11_BUFFER_DESC colorBufferDesk = {};
	ZeroMemory(&colorBufferDesk, sizeof(D3D11_BUFFER_DESC));
	colorBufferDesk.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesk.ByteWidth = sizeof(ConstantBuffer);
	colorBufferDesk.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesk.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesk.MiscFlags = 0;
	colorBufferDesk.StructureByteStride = 0;

	// �萔�o�b�t�@�̐ݒ�
	if (FAILED(m_device->CreateBuffer(&colorBufferDesk, nullptr, _colorBuffer.GetAddressOf())))
	{
		MessageBox(NULL, L"�J���[�萔�o�b�t�@���쐬�ł��܂���ł����B", L"�G���[�E�B���h�E", MB_OK | MB_ICONERROR);
		return;
	}

	// �����l�ݒ�
	SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));


	// �T���v���[�X�e�[�g���쐬���Z�b�g����
	{
		// �ٕ����t�B���^�����O��ԁAWrap���[�h
		D3D11_SAMPLER_DESC desc = {};
		//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���`�t�B���^�����O
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// �|�C���g�T���v�����O
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// �e�N�X�`���A�h���b�V���O���[�h��Wrap��
		desc.MipLODBias = 0;
		desc.MaxAnisotropy = 0;
		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		desc.BorderColor[0] = desc.BorderColor[1] = desc.BorderColor[2] = desc.BorderColor[3] = 0;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		// �X�e�[�g�I�u�W�F�N�g�쐬
		ComPtr<ID3D11SamplerState> state;
		m_device->CreateSamplerState(&desc, &state);

		// �e�V�F�[�_�[��0�ԖڂɃZ�b�g(���ۂ͕K�v�ȃV�F�[�_�[�����Z�b�g���Ă�������)
		m_deviceContext->VSSetSamplers(0, 1, state.GetAddressOf()); // ���_�V�F�[�_�[��0�ԖڂɃZ�b�g
		m_deviceContext->PSSetSamplers(0, 1, state.GetAddressOf()); // �s�N�Z���V�F�[�_�[��0�ԖڂɃZ�b�g
		m_deviceContext->GSSetSamplers(0, 1, state.GetAddressOf()); // �W�I���g���V�F�[�_�[��0�ԖڂɃZ�b�g
		m_deviceContext->CSSetSamplers(0, 1, state.GetAddressOf()); // �R���s���[�g�V�F�[�_�[��0�ԖڂɃZ�b�g
	}

	// �u�����h�̐ݒ�
	D3D11_BLEND_DESC blendDesc; // �u�����h��Ԃɂ��Đݒ肷��\����
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Alpha�l��S�ẴJ���[�v�f�ɏ�Z����
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// �u�����h�X�e�[�g���쐬
	ComPtr<ID3D11BlendState> blendState;
	if (FAILED(m_device->CreateBlendState(&blendDesc, blendState.GetAddressOf())))
	{
		MessageBox(NULL, L"�u�����h�X�e�[�g���쐬�ł��܂���ł����B", L"�G���[�E�B���h�E", MB_OK | MB_ICONERROR);
		return;
	}

	// �u�����h���Z�b�g
	FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	m_deviceContext->OMSetBlendState(blendState.Get(), blendFactor, 0xffffffff);
}

void Direct3D::SetRect(float x, float y, float w, float h)
{
	float hW = w * 0.5f;
	float hH = h * 0.5f;

	// ���_�f�[�^�쐬
	VertexType2D v[4] = {
		{{x - hW, y - hH, 0}, {0, 1}},	// ����
		{{x - hW, y + hH, 0}, {0, 0}},	// ����
		{{x + hW, y - hH, 0}, {1, 1}},	// �E��
		{{x + hW, y + hH, 0}, {1, 0}},	// �E��
	};

	// ���_�o�b�t�@�Ƀf�[�^����������
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(m_deviceContext->Map(m_vbSquare.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// �f�[�^�R�s�[
		memcpy_s(pData.pData, sizeof(v), &v[0], sizeof(v));

		m_deviceContext->Unmap(m_vbSquare.Get(), 0);
	}
}

void Direct3D::SetRect(float x, float y, float w, float h, Quaternion quaternion)
{
	float hW = w * 0.5f;
	float hH = h * 0.5f;

	Vector3 position = Camera::WorldToViewportPoint(Vector3(x, y, 0));

	Vector3 direction_leftDown = Vector3(-hW, -hH, 0);
	Vector3 direction_leftUp = Vector3(-hW, hH, 0);
	Vector3 direction_rightDown = Vector3(hW, -hH, 0);
	Vector3 direction_rightUp = Vector3(hW, hH, 0);

	Vector3 leftDown = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_leftDown));
	Vector3 leftUp = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_leftUp));
	Vector3 rightDown = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_rightDown));
	Vector3 rightUp = position + Camera::WorldToViewportPoint(quaternion.Mult(direction_rightUp));

	// ���_�f�[�^�쐬
	VertexType2D v[4] = {
		{{leftDown.x, leftDown.y, 0}, {0, 1}},	// ����
		{{leftUp.x, leftUp.y, 0}, {0, 0}},	// ����
		{{rightDown.x, rightDown.y, 0}, {1, 1}},	// �E��
		{{rightUp.x, rightUp.y, 0}, {1, 0}},	// �E��
	};

	// ���_�o�b�t�@�Ƀf�[�^����������
	D3D11_MAPPED_SUBRESOURCE pData;
	// Map...�T�u���\�[�X�Ɋ܂܂��f�[�^�ւ̃|�C���^�[���擾���A���̃T�u���\�[�X�ւ� GPU �A�N�Z�X�����ۂ��܂��B
	if (SUCCEEDED(m_deviceContext->Map(m_vbSquare.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ���_�f�[�^���T�u���\�[�X�ɃR�s�[
		memcpy_s(pData.pData, sizeof(v), &v[0], sizeof(v));

		m_deviceContext->Unmap(m_vbSquare.Get(), 0);
	}
}

void Direct3D::SetColor(DirectX::XMFLOAT4 color)
{
	// �X�V����f�[�^��p��
	ColorBuffer colorBuffer;
	colorBuffer.color = color;

	// �萔�o�b�t�@�Ƀf�[�^����������
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	if (SUCCEEDED(m_deviceContext->Map(_colorBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
	{
		memcpy(mappedSubresource.pData, &colorBuffer, sizeof(ColorBuffer));
		m_deviceContext->Unmap(_colorBuffer.Get(), 0);
	}
	else
	{
		MessageBox(NULL, L"�J���[�ݒ莞�ɁA�萔�o�b�t�@���X�V�ł��܂���ł����B", L"�G���[�E�B���h�E", MB_OK | MB_ICONERROR);
		return;
	}

	// �s�N�Z���V�F�[�_�[�ɒ萔�o�b�t�@��ݒ�
	m_deviceContext->PSSetConstantBuffers(1, 1, _colorBuffer.GetAddressOf());
}

void Direct3D::Draw2D()
{
	m_deviceContext->VSSetShader(_colorShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_colorShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_colorShader->GetInputLayout().Get());

	m_deviceContext->Draw(4, 0);
}

void Direct3D::Draw2D(const Texture& texture)
{
	m_deviceContext->VSSetShader(_textureShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_textureShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_textureShader->GetInputLayout().Get());

	// �e�N�X�`�����A�s�N�Z���V�F�[�_�[�̃X���b�g0�ɃZ�b�g
	m_deviceContext->PSSetShaderResources(0, 1, texture.m_shaderResourceview.GetAddressOf());

	// �f�o�C�X�R���e�L�X�g����A��L�̃Z�b�g�������e�ŕ`�悵�Ă��������A�Ƃ��肢����
	m_deviceContext->Draw(4, 0); // ���_�̐�
}

void Direct3D::DrawChar(ComPtr<ID3D11ShaderResourceView> shaderResourceView)
{
	m_deviceContext->VSSetShader(_textureShader->GetVertexShader().Get(), 0, 0);
	m_deviceContext->PSSetShader(_textureShader->GetPixelShader().Get(), 0, 0);
	m_deviceContext->IASetInputLayout(_textureShader->GetInputLayout().Get());

	// �e�N�X�`�����A�s�N�Z���V�F�[�_�[�̃X���b�g0�ɃZ�b�g
	m_deviceContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());

	// �f�o�C�X�R���e�L�X�g����A��L�̃Z�b�g�������e�ŕ`�悵�Ă��������A�Ƃ��肢����
	m_deviceContext->Draw(4, 0); // ���_�̐�
}