#include "Camera.h"

const kmuvcl::math::vec3f Camera::center_position() const
{

  kmuvcl::math::vec3f center_position(
		position_(0) + front_dir_(0),
                position_(1) + front_dir_(1), 
                position_(2) + front_dir_(2));
  return center_position;
}


// TODO: fill up the following functions properly 
void Camera::move_forward(float delta)
{	
  position_ += delta * front_dir_;
  position_(1) = 3;
}

void Camera::move_backward(float delta)
{	
  position_ -= delta * front_dir_;
  position_(1) = 3;
}

void Camera::move_left(float delta)
{	

  position_ -= delta * right_dir_;
  position_(1) = 3;
}

void Camera::move_right(float delta)
{

  position_ += delta * right_dir_;
  position_(1) = 3;
}

void Camera::rotate_left(float delta)
{
  kmuvcl::math::mat4x4f R;
  R = kmuvcl::math::rotate(delta, 0.0f, 1.0f, 0.0f);

  kmuvcl::math::vec4f rotate_, front_(front_dir_(0), front_dir_(1), front_dir_(2), 0);
  rotate_ = R*front_;

  front_dir_(0) = rotate_(0);
  front_dir_(1) = rotate_(1);
  front_dir_(2) = rotate_(2);

  right_dir_ = kmuvcl::math::cross(front_dir_, up_dir_);
}

void Camera::rotate_right(float delta)
{
  kmuvcl::math::mat4x4f R;
  R = kmuvcl::math::rotate(-delta, 0.0f, 1.0f, 0.0f);

  kmuvcl::math::vec4f rotate_, front_(front_dir_(0), front_dir_(1), front_dir_(2), 0);
  rotate_ = R*front_;

  front_dir_(0) = rotate_(0);
  front_dir_(1) = rotate_(1);
  front_dir_(2) = rotate_(2);

  right_dir_ = kmuvcl::math::cross(front_dir_, up_dir_);
}

void Camera::rotate_up(float delta)
{
  kmuvcl::math::mat4x4f R;
  kmuvcl::math::vec4f rotate_, front_(front_dir_(0), front_dir_(1), front_dir_(2), 0);; 

  R = kmuvcl::math::rotate(delta, right_dir_(0), right_dir_(1), right_dir_(2));
  rotate_ = R*front_;

  front_dir_(0) = rotate_(0);
  front_dir_(1) = rotate_(1);
  front_dir_(2) = rotate_(2);

  right_dir_ = kmuvcl::math::cross(front_dir_, up_dir_);

  position_(1) = 3;
}
void Camera::rotate_down(float delta)
{

  kmuvcl::math::mat4x4f R;
  kmuvcl::math::vec4f rotate_, front_(front_dir_(0), front_dir_(1), front_dir_(2), 0);; 

  R = kmuvcl::math::rotate(-delta, right_dir_(0), right_dir_(1), right_dir_(2));
  rotate_ = R*front_;

  front_dir_(0) = rotate_(0);
  front_dir_(1) = rotate_(1);
  front_dir_(2) = rotate_(2);

  right_dir_ = kmuvcl::math::cross(front_dir_, up_dir_);

  position_(1) = 3;
}

void Camera::reset()
{
  position_(0) = 0;
  position_(1) = 3;
  position_(2) = 10;

  front_dir_(0) = 0;
  front_dir_(1) = 0;
  front_dir_(2) = -1;

  up_dir_(0) = 0;
  up_dir_(1) = 1;
  up_dir_(2) = 0;

  right_dir_(0) = 1;
  right_dir_(1) = 0;
  right_dir_(2) = 0;
}

