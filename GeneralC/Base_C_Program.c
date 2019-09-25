//  qemu-io-cmds.c


static QemuOptsList reopen_opts = {
    .name = "reopen",
    .merge_lists = true,
    .head = QTAILQ_HEAD_INITIALIZER(reopen_opts.head),
    .desc = {
        /* no elements => accept any params */
        {/* end of list */}},
};

static void reopen_help(void)
{
    printf(
        "\n"
        " Changes the open options of an already opened image\n"
        "\n"
        " Example:\n"
        " 'reopen -o lazy-refcounts=on' - activates lazy refcount writeback on a qcow2 image\n"
        "\n"
        " -r, -- Reopen the image read-only\n"
        " -c, -- Change the cache mode to the given value\n"
        " -o, -- Changes block driver options (cf. 'open' command)\n"
        "\n");
}

static int reopen_f(BlockBackend *blk, int argc, char **argv);

static const cmdinfo_t reopen_cmd = {
    .name = "reopen",
    .argmin = 0,
    .argmax = -1,
    .cfunc = reopen_f,
    .args = "[-r] [-c cache] [-o options]",
    .oneline = "reopens an image with new options",
    .help = reopen_help,
};

static int reopen_f(BlockBackend *blk, int argc, char **argv)
{
    BlockDriverState *bs = blk_bs(blk);
    QemuOpts *qopts;
    QDict *opts;
    int c;
    int flags = bs->open_flags;

    BlockReopenQueue *brq;
    Error *local_err = NULL;

    while ((c = getopt(argc, argv, "c:o:r")) != -1)
    {
        switch (c)
        {
        case 'c':
            if (bdrv_parse_cache_flags(optarg, &flags) < 0)
            {
                error_report("Invalid cache option: %s", optarg);
                return 0;
            }
            break;
        case 'o':
            if (!qemu_opts_parse_noisily(&reopen_opts, optarg, 0))
            {
                qemu_opts_reset(&reopen_opts);
                return 0;
            }
            break;
        case 'r':
            flags &= ~BDRV_O_RDWR;
            break;
        default:
            qemu_opts_reset(&reopen_opts);
            return qemuio_command_usage(&reopen_cmd);
        }
    }

    if (optind != argc)
    {
        qemu_opts_reset(&reopen_opts);
        return qemuio_command_usage(&reopen_cmd);
    }

    qopts = qemu_opts_find(&reopen_opts, NULL);
    opts = qopts ? qemu_opts_to_qdict(qopts, NULL) : NULL;
    qemu_opts_reset(&reopen_opts);

    brq = bdrv_reopen_queue(NULL, bs, opts, flags);
    bdrv_reopen_multiple(brq, &local_err);
    if (local_err)
    {
        error_report_err(local_err);
    }

    return 0;
}

static int command(BlockBackend * blk, const cmdinfo_t *ct, int argc, char **argv)
{
    char *cmd = argv[0];

    if (!init_check_command(blk, ct))
    {
        return 0;
    }

    if (argc - 1 < ct->argmin || (ct->argmax != -1 && argc - 1 > ct->argmax))
    {
        if (ct->argmax == -1)
        {
            fprintf(stderr,
                    "bad argument count %d to %s, expected at least %d arguments\n",
                    argc - 1, cmd, ct->argmin);
        }
        else if (ct->argmin == ct->argmax)
        {
            fprintf(stderr,
                    "bad argument count %d to %s, expected %d arguments\n",
                    argc - 1, cmd, ct->argmin);
        }
        else
        {
            fprintf(stderr,
                    "bad argument count %d to %s, expected between %d and %d arguments\n",
                    argc - 1, cmd, ct->argmin, ct->argmax);
        }
        return 0;
    }
    optind = 0;
    return ct->cfunc(blk, argc, argv);
}

bool qemuio_command(BlockBackend *blk, const char *cmd)
{
    char *input;
    const cmdinfo_t *ct;
    char **v;
    int c;
    bool done = false;

    input = g_strdup(cmd);
    v = breakline(input, &c);
    if (c)
    {
        ct = find_command(v[0]);
        if (ct)
        {
            done = command(blk, ct, c, v);
        }
        else
        {
            fprintf(stderr, "command \"%s\" not found\n", v[0]);
        }
    }
    g_free(input);
    g_free(v);

    return done;
}

static const cmdinfo_t *find_command(const char *cmd)
{
    cmdinfo_t *ct;

    for (ct = cmdtab; ct < &cmdtab[ncmds]; ct++)
    {
        if (strcmp(ct->name, cmd) == 0 ||
            (ct->altname && strcmp(ct->altname, cmd) == 0))
        {
            return (const cmdinfo_t *)ct;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    return 
}