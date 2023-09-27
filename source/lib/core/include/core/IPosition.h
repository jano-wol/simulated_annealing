#include <core/IMove.h>
#include <memory>

class IPosition
{
public:
  virtual double getEnergy() = 0;
  virtual std::shared_ptr<IMove> getMove() = 0;
  virtual std::shared_ptr<IPosition> makeMove(std::shared_ptr<IMove>) = 0;
};
