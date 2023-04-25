#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zenoh.h>

static volatile bool done = false;

static void lib_subscribe_callback(const z_sample_t* sample, void* arg)
{
    (void)sample;
    (void)arg;

    fprintf(stdout, "%s\n", __PRETTY_FUNCTION__);
    fflush(stdout);

    done = true;
}

static z_owned_session_t make_zenoh_session()
{
    const char* const remote = "[\"tcp/localhost:7447\"]";

    z_owned_config_t config = z_config_default();
    zc_config_insert_json(z_config_loan(&config), Z_CONFIG_CONNECT_KEY, remote);
    zc_config_insert_json(z_config_loan(&config), Z_CONFIG_MODE_KEY, "\"client\"");
    zc_config_insert_json(z_config_loan(&config), Z_CONFIG_MULTICAST_SCOUTING_KEY, "false");
    zc_config_insert_json(z_config_loan(&config), "timestamping/enabled", "true");

    return z_open(z_move(config));
}

int main()
{
    z_owned_session_t put_session = make_zenoh_session();
    z_owned_session_t sub_sesssion = make_zenoh_session();

    const z_keyexpr_t keyexpr = z_keyexpr("zenoh-debug/int");

    const z_put_options_t put_opts = {
        .encoding = z_encoding(Z_ENCODING_PREFIX_APP_INTEGER, ""),
        .congestion_control = Z_CONGESTION_CONTROL_BLOCK,
        .priority = Z_PRIORITY_REAL_TIME};

    const z_subscriber_options_t sub_opts = {.reliability = Z_RELIABILITY_RELIABLE};

    z_owned_closure_sample_t closure = {
        .context = NULL,
        .call = lib_subscribe_callback,
        .drop = NULL};

    z_owned_subscriber_t sub =
        z_declare_subscriber(z_loan(sub_sesssion), keyexpr, z_move(closure), &sub_opts);

    // This should not be here
    // usleep(1000);

    int8_t res = z_put(z_loan(put_session), keyexpr, (const uint8_t*)("1234"), 4, &put_opts);
    fprintf(stdout, "z_put returned %d\n", res);
    fflush(stdout);

    while (!done) {
        usleep(1000);
    }

    z_undeclare_subscriber(z_move(sub));
}
