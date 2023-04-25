#pragma once

#include <Windows.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

auto is_code_ptr(void* ptr) -> bool {
	constexpr const DWORD protect_flags{ PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY };

	MEMORY_BASIC_INFORMATION out;
	::VirtualQuery(ptr, &out, sizeof(out));

	return out.Type && !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && out.Protect & protect_flags;
}

class table_hook {
public:
	constexpr table_hook(): m_new_vmt{ nullptr } , m_old_vmt{ nullptr } {}

	~table_hook() {
		if (m_new_vmt)
			delete[](m_new_vmt - 1);
	}

protected:
	auto initialize(void** original_table) -> void {
		m_old_vmt = original_table;

		std::size_t size{ 0 };
		while (m_old_vmt[size] && is_code_ptr(m_old_vmt[size]))
			++size;

		m_new_vmt = (new void* [size + 1]) + 1;
		::memcpy(m_new_vmt - 1, m_old_vmt - 1, sizeof(void*) * (size + 1));
	}

	constexpr auto leak_table() -> void { m_new_vmt = nullptr; }

	auto hook_instance(void* inst) const -> void {
		auto& vtbl{ *reinterpret_cast<void***>(inst) };
		assert(vtbl == m_old_vmt || vtbl == m_new_vmt);
		vtbl = m_new_vmt;
	}

	auto unhook_instance(void* inst) const -> void {
		auto& vtbl{ *reinterpret_cast<void***>(inst) };
		assert(vtbl == m_old_vmt || vtbl == m_new_vmt);
		vtbl = m_old_vmt;
	}

	auto initialize_and_hook_instance(void* inst) -> bool {
		auto& vtbl{ *reinterpret_cast<void***>(inst) };
		bool initialized{ false };
		if (!m_old_vmt) {
			initialized = true;
			initialize(vtbl);
		}
		hook_instance(inst);
		return initialized;
	}

	template <typename Fn>
	auto hook_function(Fn hooked_fn, const std::size_t index) -> Fn {
		m_new_vmt[index] = (void*)(hooked_fn);
		return (Fn)(m_old_vmt[index]);
	}

	template<typename T>
	auto apply_hook(std::size_t idx) -> void { T::m_original = hook_function(&T::hooked, idx); }

	template <typename Fn = std::uintptr_t>
	auto get_original_function(const int index) -> Fn { return (Fn)(m_old_vmt[index]); }

private:
	void** m_new_vmt{ nullptr };
	void** m_old_vmt{ nullptr };
};

class vmt_smart_hook : table_hook {
public:
	vmt_smart_hook(void* class_base) : m_class{ class_base } { initialize_and_hook_instance(class_base); }
	~vmt_smart_hook() { unhook_instance(m_class); }

	auto rehook() const -> void { hook_instance(m_class); }
	auto unhook() const -> void { unhook_instance(m_class); }

	using table_hook::apply_hook;
	using table_hook::get_original_function;
	using table_hook::hook_function;

private:
	void* m_class{ nullptr };
};

class vmt_multi_hook : table_hook {
public:
	constexpr vmt_multi_hook() { }

	~vmt_multi_hook() { leak_table(); }

	using table_hook::apply_hook;
	using table_hook::get_original_function;
	using table_hook::hook_function;
	using table_hook::hook_instance;
	using table_hook::unhook_instance;
	using table_hook::initialize;
	using table_hook::initialize_and_hook_instance;
};
