namespace hbt::repo {
template <typename T> class SingleItemRepository {
  public:
    virtual auto save(const T &data) -> bool = 0;

    virtual auto load() & -> const T & = 0;
};

template <typename T, typename TID> class MultiItemRepository {
  public:
    virtual auto save(const T &data) -> bool = 0;

    virtual auto load(TID id) & -> const T & = 0;
};
} // namespace hbt::repo
