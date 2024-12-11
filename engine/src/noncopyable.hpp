#pragma once

namespace world {

  class NonCopyable
  {
  public:
      NonCopyable() = default;
      virtual ~NonCopyable() = default;
      
      NonCopyable(NonCopyable const&) = delete;
      auto operator=(const NonCopyable&) -> NonCopyable& = delete;
      NonCopyable(NonCopyable&&) = delete;
      auto operator=(NonCopyable&&) -> NonCopyable& = delete;
  };

}