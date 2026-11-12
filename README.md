<div align="center">

# R3nzSkin

**&gt; English &lt;** | [简体中文](README_zh.md)

</div>

> [!IMPORTANT]
> Please do not use Issues to post advertisements, promotions, or repeated recommendations of similar paid products. Whether those products work or not is unrelated to this project, but abusive language, spam, or repeatedly posting through alternate accounts will not be accepted.




# **Disclaimer**

**This project is for learning and technical exchange purposes only. Commercial use or any illegal activity is strictly prohibited. Any direct or indirect consequences arising from the use of this project shall be borne solely by the user, and the author assumes no responsibility.**

**By using this project, you fully understand and accept the above terms.**

# Building

1. Clone the source with `git clone --recursive https://github.com/hydy100/R3nzSkin.git`

   - **If you don't modify this source code, compiling and using it on Chinese servers will definitely result in a ban. The functionality is fine.**

2. Build in Visual Studio 2019/2022 with configuration "Your Region - x64"

   > Quick tip: The original method of RiotServers cannot be injected. The injection method I currently use is `SetWindowsHookEx`. With a little effort, the injection should be possible. There is no need to say more, and don’t ask me.

# Usage

1. Compile source or [download](https://github.com/hydy100/R3nzSkin/releases/latest) a compiled version.
2. Then check: [Instructions for use](https://hydy100.top/) ,I've written the detailed instructions inside.

# About the project

- I include some explanations for each release in the description of [Releases](https://github.com/hydy100/R3nzSkin/releases/latest).
- **I've hardly posted this project on any other platform, so if you share it, please help solve some issues rather than redirecting everything to me. Alternatively, provide [R3nzSkin](https://github.com/hydy100/R3nzSkin) so they can submit issues there.**
- **There is no paid version or any other versions, and there is no requirement to join any groups. I've released all the files, I just left a contact method.**
- The project is currently purely non-profit, just like the original [R3nzSkin](https://github.com/R3nzTheCodeGOD/R3nzSkin).
- There are some issues I can't solve very well, so I welcome anyone with the ability to help resolve them, if you're willing.

# About the releases
- Regarding the issue of dll size in releases, I just added some anti-cracking shells or compressed shells, and the dll function source code has been completely open source.
- Regarding the injector part in releases, it is not open source, but the quick tip also mentioned how to update it to make it available. It was not made clear before that the original version cannot be used, but the fact that I have never submitted any updates for the injector also indicates this issue.
- The releases and source codes of many open source projects are also different. I don't want anyone to kidnap me in the name of open source and ask me to do something meaningless. This goes against the original intention of open source.
- **Due to the existence of special risks, in order to avoid unnecessary trouble, the file release of the Chinese server version may be stopped in the future.**

# Regarding views on open source (quoting ChatGPT)
If someone tries to "hijack" you through the open-source spirit, demanding that you justify the rationale behind parts of the code that are not open-source, this actually contradicts the core values of the open-source spirit. The essence of open source is to encourage sharing and collaboration, but this doesn't mean every line of code has to be open. Open-source licenses allow developers to decide whether or not to make their code public, and in some cases, certain parts of the code may not be suitable for open-source due to commercial, privacy, or security reasons.

Here are a few points to understand this:

1. Balance between open-source and private code: Open-source does not require all code to be open. In many open-source projects, the core functionality or sensitive parts might remain private, while other parts are made public. Developers have the right to decide which parts to share and which to keep private.

2. Respecting the developer's choice: If a developer chooses not to open-source a part of their code, that decision should be respected. It does not mean that the developer is not adhering to the open-source spirit; they may have other reasons, such as protecting commercial interests, maintaining security, or preventing abuse.

3. Avoiding "hijacking" behavior: The open-source community's spirit is based on freedom, collaboration, and respect. If someone uses the "open-source spirit" as a pretext to force you to explain or change your code-sharing strategy, this behavior itself deviates from the true meaning of open source. Open-source is voluntary, and any form of coercion is not to be encouraged.

In summary, the open-source spirit advocates freedom, sharing, and collaboration, but it does not mean that all code must be open. Every developer should have the right to decide the level of openness for their own code.


# Further optimizations

If your CPU supports AVX / AVX2 / AVX-512 instruction set, you can enable it in project settings. This should result in more performant code, optimized for your CPU. Currently SSE2 instructions are selected in project settings.

# Credits

This program is an improved and updated version of the [R3nzTheCodeGOD](https://github.com/R3nzTheCodeGOD)/[R3nzSkin](https://github.com/R3nzTheCodeGOD/R3nzSkin) project.
