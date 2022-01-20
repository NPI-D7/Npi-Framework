#include "rn2d.hpp"

rn2d::rn2d()
{
    rn2d = std::make_unique<Npi::Shader>(rn2d_shbin, rn2d_shbin_size, Npi::Shader::Type::VERTEX);
}
