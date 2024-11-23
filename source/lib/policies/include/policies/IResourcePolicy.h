namespace sa::policies
{
class IResourcePolicy
{
public:
  virtual double getResourceAll() const = 0;
  virtual double getResourceLeft() const = 0;
  virtual void updateResourceLeft() = 0;
  virtual ~IResourcePolicy() = default;
};
}  // namespace sa::policies