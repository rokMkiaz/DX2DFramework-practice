#pragma once

class Camera final
{
public:
	Camera();
	~Camera() = default;

	auto GetViewMatrix() const -> const D3DXMATRIX& { return view; }
	auto GetProjectionMatrix() const -> const D3DXMATRIX& { return proj; }
	
	auto GetPosition()  const -> const D3DXVECTOR3& { return position; }
	void SetPosition(const D3DXVECTOR3& position) { this->position = position; }

	void Update();

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

private:
	D3DXMATRIX view;
	D3DXMATRIX proj;
	D3DXVECTOR3 position = D3DXVECTOR3(0,0,0);

};