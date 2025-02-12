#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include "resultline.h"

class ResultsWidget : public QWidget
{
    Q_OBJECT

    QVBoxLayout *main_layout = nullptr;
    QGroupBox *result_box = nullptr;
    QVBoxLayout *groupbox_layout = nullptr;

    ResultLine *rl_sld = nullptr; ResultLine *rl_sld_im = nullptr;
    ResultLine *rl_pot_v = nullptr; ResultLine *rl_pot_v_im = nullptr;
    ResultLine *rl_ch_wl = nullptr; ResultLine *rl_cr_ang = nullptr;
    ResultLine *rl_cr_mom = nullptr; ResultLine *rl_atl_c = nullptr;
    ResultLine *rl_absorb = nullptr; ResultLine *rl_scatt = nullptr;

    void initialize();
    void set_resultlines();
    void construct();

public:
    explicit ResultsWidget(QWidget *parent = nullptr);

    void clear();

    void set_re_sld(double v, double e)     { rl_sld->set_result(v, e); }
    void set_im_sld(double v, double e)     { rl_sld_im->set_result(v, e); }
    void set_re_v(double v, double e)       { rl_pot_v->set_result(v, e); }
    void set_im_v(double v, double e)       { rl_pot_v_im->set_result(v, e); }

    void set_lambda_c(double v, double e)   { rl_ch_wl->set_result(v, e); }
    void set_tetta_c(double v, double e)    { rl_cr_ang->set_result(v, e); }
    void set_q_c(double v, double e)        { rl_cr_mom->set_result(v, e); }
    void set_mu(double v, double e)         { rl_atl_c->set_result(v, e); }
    void set_mu_a(double v , double e)      { rl_absorb->set_result(v, e); }
    void set_mu_inc(double v, double e)     { rl_scatt->set_result(v, e); }

signals:
};

#endif // RESULTSWIDGET_H
