#include "primewindow.h"
#include "opengl.h"
#include "ui_primewindow.h"
#include "xdg.h"
#include "vulkan.h"

primewindow::primewindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::primewindow)
    , m_config(prime::get_xdg_config() + "/prime", "prime.cfg")
{
    m_ui->setupUi(this);
    m_ui->env_button_box->button(QDialogButtonBox::Save)->setEnabled(false);

    connect(
        m_ui->quit_button, &QPushButton::clicked, this, [this](auto checked) { this->close(); });
    connect(
        m_ui->run_test_button, &QPushButton::clicked, this, [this](auto checked) { run_tests(); });
    connect(m_ui->env_button_box, &QDialogButtonBox::clicked, this, [this](auto button) {
        if (button == m_ui->env_button_box->button(QDialogButtonBox::Reset)) {
            init_env();
        }
        if (button == m_ui->env_button_box->button(QDialogButtonBox::RestoreDefaults)) {
            restore_defaults();
        }
        if (button == m_ui->env_button_box->button(QDialogButtonBox::Save)) {
            save_config();
        }
    });

    m_lines = {m_ui->env_prime_line,
               m_ui->env_glx_line,
               m_ui->env_layer_line,
               m_ui->env_render_line,
               m_ui->env_provider_line,
               m_ui->env_icd_line,
               m_ui->env_egl_line,
               m_ui->env_dxvk_line,
               m_ui->env_vkd3d_line};
    init_env();

    for (auto const& line : m_lines) {
        connect(line, &QLineEdit::textChanged, this, [this](auto text) {
            m_ui->env_button_box->button(QDialogButtonBox::Save)->setEnabled(true);
        });
    }
}

primewindow::~primewindow()
{
    delete m_ui;
}

void primewindow::init_env()
{
    m_config.load_config();
    auto cfg_map = m_config.get_config_map();
    if (!cfg_map.empty()) {
        for (auto const& [key, val] : cfg_map) {
            if (key == "DRI_PRIME") {
                m_ui->env_prime_line->setText(QString::fromStdString(val));
            }
            if (key == "__GLX_VENDOR_LIBRARY_NAME") {
                m_ui->env_glx_line->setText(QString::fromStdString(val));
            }
            if (key == "__VK_LAYER_NV_optimus") {
                m_ui->env_layer_line->setText(QString::fromStdString(val));
            }
            if (key == "__NV_PRIME_RENDER_OFFLOAD") {
                m_ui->env_render_line->setText(QString::fromStdString(val));
            }
            if (key == "__NV_PRIME_RENDER_OFFLOAD_PROVIDER") {
                m_ui->env_provider_line->setText(QString::fromStdString(val));
            }
            if (key == "VK_ICD_FILENAMES") {
                m_ui->env_icd_line->setText(QString::fromStdString(val));
            }
            if (key == "_EGL_VENDOR_LIBRARY_FILENAMES") {
                m_ui->env_egl_line->setText(QString::fromStdString(val));
            }
            if (key == "DXVK_FILTER_DEVICE_NAME") {
                m_ui->env_dxvk_line->setText(QString::fromStdString(val));
            }
            if (key == "VKD3D_FILTER_DEVICE_NAME") {
                m_ui->env_vkd3d_line->setText(QString::fromStdString(val));
            }
        }
    }

    for (auto& line : m_lines) {
        if (line->text().isEmpty()) {
            line->setText("Environment not set.");
        }
    }
}

void primewindow::run_tests()
{
    m_ui->output_text_edit->clear();
    auto init_gl = [this](auto& gl) {
        if (!gl.create_instance()) {
            m_ui->output_text_edit->insertPlainText("Cant create a OpenGL instance.\n");
            return false;
        }

        if (gl.get_devices().empty()) {
            m_ui->output_text_edit->insertPlainText("Found no OpenGL device.\n");
            return false;
        }

        return true;
    };

    auto init_vulkan = [this](auto& vulkan) {
        if (!vulkan.create_instance()) {
            m_ui->output_text_edit->insertPlainText("Cant create a Vulkan instance.\n");
            return false;
        }

        if (vulkan.get_devices().empty()) {
            m_ui->output_text_edit->insertPlainText("Found no Vulkan device.\n");
            return false;
        }

        return true;
    };

    m_config.load_config();
    if (m_ui->env_var_checkbox->isChecked()) {
        // set env vars
        auto cfg_map = m_config.get_config_map();
        for (auto const& [key, val] : cfg_map) {
            prime::set_env_var(key, val, 1);
        }
    } else {
        // unset the env vars incase this is rerun with the vars already set once.
        auto cfg_map = m_config.get_config_map();
        for (auto const& [key, val] : cfg_map) {
            prime::unset_env_var(key);
        }
    }

    if (m_ui->opengl_checkbox->isChecked()) {
        prime::opengl gl;

        if (init_gl(gl)) {
            auto dev = gl.get_devices().front();
            m_ui->output_text_edit->insertPlainText("[Primary OpenGL device]\n");
            m_ui->output_text_edit->insertPlainText(
                "Vendor: " + QString::fromStdString(dev.get_vendor() + "\n"));
            m_ui->output_text_edit->insertPlainText(
                "Renderer: " + QString::fromStdString(dev.get_renderer() + "\n"));
            m_ui->output_text_edit->insertPlainText(
                "Version: " + QString::fromStdString(dev.get_version() + "\n"));
            m_ui->output_text_edit->insertPlainText(
                "Shading Version: " + QString::fromStdString(dev.get_shading_version() + "\n\n"));
        }
    }

    if (m_ui->vulkan_checkbox->isChecked()) {
        prime::vulkan vulkan;
        if (init_vulkan(vulkan)) {
            auto dev = vulkan.get_devices().front();
            auto prop = vulkan.get_property(dev);
            auto driver_version = vulkan.get_version(prop.driverVersion);
            auto api_version = vulkan.get_version(prop.apiVersion);

            m_ui->output_text_edit->insertPlainText("[Primary Vulkan device]\n");
            m_ui->output_text_edit->insertPlainText("Vendor: " + QString(prop.deviceName) + "\n");
            m_ui->output_text_edit->insertPlainText(
                "Driver Version: " + QString::fromStdString(driver_version) + "\n");
            m_ui->output_text_edit->insertPlainText(
                "Vulkan API Version: " + QString::fromStdString(api_version) + "\n");
        }
    }
}

void primewindow::restore_defaults()
{
    m_ui->env_prime_line->setText("1");
    m_ui->env_glx_line->setText("nvidia");
    m_ui->env_layer_line->setText("NVIDIA_only");
    m_ui->env_render_line->setText("1");
    m_ui->env_provider_line->setText("NVIDIA-G0");
    m_ui->env_icd_line->setText("/usr/share/vulkan/icd.d/nvidia_icd.json");
    m_ui->env_egl_line->setText("/usr/share/glvnd/egl_vendor.d/10_nvidia.json");
    m_ui->env_dxvk_line->setText("NVIDIA");
    m_ui->env_vkd3d_line->setText("NVIDIA");
}

void primewindow::save_config()
{
    std::map<std::string, std::string> map;
    auto get_line = [&map](auto env, QLineEdit* line) {
        if (!line->text().isEmpty() && line->text() != "Environment not set.") {
            map.insert(std::pair<std::string, std::string>(env, line->text().toStdString()));
        }
    };

    get_line("DRI_PRIME", m_ui->env_prime_line);
    get_line("__GLX_VENDOR_LIBRARY_NAME", m_ui->env_glx_line);
    get_line("__VK_LAYER_NV_optimus", m_ui->env_layer_line);
    get_line("__NV_PRIME_RENDER_OFFLOAD", m_ui->env_render_line);
    get_line("__NV_PRIME_RENDER_OFFLOAD_PROVIDER", m_ui->env_provider_line);
    get_line("VK_ICD_FILENAMES", m_ui->env_icd_line);
    get_line("_EGL_VENDOR_LIBRARY_FILENAMES", m_ui->env_egl_line);
    get_line("DXVK_FILTER_DEVICE_NAME", m_ui->env_dxvk_line);
    get_line("VKD3D_FILTER_DEVICE_NAME", m_ui->env_vkd3d_line);

    m_config.write_config_map(map);
    m_ui->env_button_box->button(QDialogButtonBox::Save)->setEnabled(false);
}
