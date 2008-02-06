#include <ruby.h>
#include "libruby_module.h"
#include "dbm.h"

VALUE cRegChannel = Qnil;

static VALUE RegChannel_Initialize(VALUE, VALUE);
static VALUE RegChannel_Id(VALUE);
static VALUE RegChannel_IdSet(VALUE, VALUE);
static VALUE RegChannel_Name(VALUE);
static VALUE RegChannel_NameSet(VALUE, VALUE);
static VALUE RegChannel_Description(VALUE);
static VALUE RegChannel_DescriptionSet(VALUE, VALUE);
static VALUE RegChannel_EntryMsg(VALUE);
static VALUE RegChannel_EntryMsgSet(VALUE, VALUE);
static VALUE RegChannel_Url(VALUE);
static VALUE RegChannel_UrlSet(VALUE, VALUE);
static VALUE RegChannel_Email(VALUE);
static VALUE RegChannel_EmailSet(VALUE, VALUE);
static VALUE RegChannel_Topic(VALUE);
static VALUE RegChannel_TopicSet(VALUE, VALUE);
static VALUE RegChannel_Mlock(VALUE);
static VALUE RegChannel_MlockSet(VALUE, VALUE);
static VALUE RegChannel_Priv(VALUE);
static VALUE RegChannel_PrivSet(VALUE, VALUE);
static VALUE RegChannel_Restricted(VALUE);
static VALUE RegChannel_RestrictedSet(VALUE, VALUE);
static VALUE RegChannel_TopicLock(VALUE);
static VALUE RegChannel_TopicLockSet(VALUE, VALUE);
static VALUE RegChannel_Verbose(VALUE);
static VALUE RegChannel_VerboseSet(VALUE, VALUE);
static VALUE RegChannel_AutoLimit(VALUE);
static VALUE RegChannel_AutoLimitSet(VALUE, VALUE);
static VALUE RegChannel_ExpireBans(VALUE);
static VALUE RegChannel_ExpireBansSet(VALUE, VALUE);
static VALUE RegChannel_FloodServ(VALUE);
static VALUE RegChannel_FloodServSet(VALUE, VALUE);
/* DB */
static VALUE RegChannel_Save(VALUE);
static VALUE RegChannel_ByName(VALUE, VALUE);

static VALUE
RegChannel_Initialize(VALUE self, VALUE channel)
{
  rb_iv_set(self, "@realptr", channel);
  return self;
}

static VALUE
RegChannel_Id(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return INT2FIX(channel->id);
}

static VALUE
RegChannel_IdSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->id = FIX2INT(value);
  return value;
}

static VALUE
RegChannel_Name(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->channel);
}

static VALUE
RegChannel_NameSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  const char* cvalue;

  Check_Type(value, T_STRING);

  cvalue = StringValueCStr(value);

  strlcpy(channel->channel, StringValueCStr(value), sizeof(channel->channel));

  return value;
}

static VALUE
RegChannel_Description(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->description);
}

static VALUE
RegChannel_DescriptionSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);

  Check_Type(value, T_STRING);

  DupString(channel->description, StringValueCStr(value));
  return value;
}

static VALUE
RegChannel_EntryMsg(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->entrymsg);
}

static VALUE
RegChannel_EntryMsgSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);

  Check_Type(value, T_STRING);

  DupString(channel->entrymsg, StringValueCStr(value));
  return value;
}

static VALUE
RegChannel_Url(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->url);
}

static VALUE
RegChannel_UrlSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);

  Check_Type(value, T_STRING);

  DupString(channel->url, StringValueCStr(value));
  return value;
}

static VALUE
RegChannel_Email(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->email);
}

static VALUE
RegChannel_EmailSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);

  Check_Type(value, T_STRING);

  DupString(channel->email, StringValueCStr(value));
  return value;
}

static VALUE
RegChannel_Topic(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->topic);
}

static VALUE
RegChannel_TopicSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);

  Check_Type(value, T_STRING);

  DupString(channel->topic, StringValueCStr(value));
  return value;
}

static VALUE
RegChannel_Mlock(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return rb_str_new2(channel->mlock);
}

static VALUE
RegChannel_MlockSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);

  Check_Type(value, T_STRING);

  DupString(channel->mlock, StringValueCStr(value));
  return value;
}

static VALUE
RegChannel_Priv(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->priv ? Qtrue : Qfalse;
}

static VALUE
RegChannel_PrivSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->priv = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_Restricted(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->restricted ? Qtrue : Qfalse;
}

static VALUE
RegChannel_RestrictedSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->restricted = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_TopicLock(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->topic_lock ? Qtrue : Qfalse;
}

static VALUE
RegChannel_TopicLockSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->topic_lock = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_Verbose(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->verbose ? Qtrue : Qfalse;
}

static VALUE
RegChannel_VerboseSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->verbose = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_AutoLimit(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->autolimit ? Qtrue : Qfalse;
}

static VALUE
RegChannel_AutoLimitSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->autolimit = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_ExpireBans(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->expirebans ? Qtrue : Qfalse;
}

static VALUE
RegChannel_ExpireBansSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->expirebans = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_FloodServ(VALUE self)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  return channel->floodserv ? Qtrue : Qfalse;
}

static VALUE
RegChannel_FloodServSet(VALUE self, VALUE value)
{
  struct RegChannel *channel = rb_rbregchan2cregchan(self);
  channel->floodserv = value == Qtrue ? 1 : 0;
  return value;
}

static VALUE
RegChannel_Save(VALUE self)
{
  //struct RegChannel *channel = rb_rbregchan2cregchan(self);
  //int ret = db_save_channel(channel);
  //return ret ? Qtrue : Qfalse;
  return Qnil;
}

static VALUE
RegChannel_ByName(VALUE self, VALUE name)
{
  struct RegChannel *channel = NULL;

  Check_Type(name, T_STRING);

  //channel = db_find_chan(StringValueCStr(name));

  if(channel)
    return rb_cregchan2rbregchan(channel);
  else
    return Qnil;
}

void
Init_RegChannel(void)
{
  cRegChannel = rb_define_class("RegChannel", rb_cObject);

  rb_gc_register_address(&cRegChannel);

  rb_define_method(cRegChannel, "initialize", RegChannel_Initialize, 1);
  rb_define_method(cRegChannel, "id", RegChannel_Id, 0);
  rb_define_method(cRegChannel, "id=", RegChannel_IdSet, 1);
  rb_define_method(cRegChannel, "name", RegChannel_Name, 0);
  rb_define_method(cRegChannel, "name=", RegChannel_NameSet, 1);
  rb_define_method(cRegChannel, "description", RegChannel_Description, 0);
  rb_define_method(cRegChannel, "description=", RegChannel_DescriptionSet, 1);
  rb_define_method(cRegChannel, "entrymsg", RegChannel_EntryMsg, 0);
  rb_define_method(cRegChannel, "entrymsg=", RegChannel_EntryMsgSet, 1);
  rb_define_method(cRegChannel, "url", RegChannel_Url, 0);
  rb_define_method(cRegChannel, "url=", RegChannel_UrlSet, 1);
  rb_define_method(cRegChannel, "email", RegChannel_Email, 0);
  rb_define_method(cRegChannel, "email=", RegChannel_EmailSet, 1);
  rb_define_method(cRegChannel, "topic", RegChannel_Topic, 0);
  rb_define_method(cRegChannel, "topic=", RegChannel_TopicSet, 1);
  rb_define_method(cRegChannel, "mlock", RegChannel_Mlock, 0);
  rb_define_method(cRegChannel, "mlock=", RegChannel_MlockSet, 1);
  rb_define_method(cRegChannel, "private?", RegChannel_Priv, 0);
  rb_define_method(cRegChannel, "private=", RegChannel_PrivSet, 1);
  rb_define_method(cRegChannel, "restricted?", RegChannel_Restricted, 0);
  rb_define_method(cRegChannel, "restricted=", RegChannel_RestrictedSet, 1);
  rb_define_method(cRegChannel, "topic_lock?", RegChannel_TopicLock, 0);
  rb_define_method(cRegChannel, "topic_lock=", RegChannel_TopicLockSet, 1);
  rb_define_method(cRegChannel, "verbose?", RegChannel_Verbose, 0);
  rb_define_method(cRegChannel, "verbose=", RegChannel_VerboseSet, 1);
  rb_define_method(cRegChannel, "autolimit?", RegChannel_AutoLimit, 0);
  rb_define_method(cRegChannel, "autolimit=", RegChannel_AutoLimitSet, 1);
  rb_define_method(cRegChannel, "expirebans?", RegChannel_ExpireBans, 0);
  rb_define_method(cRegChannel, "expirebans=", RegChannel_ExpireBansSet, 1);
  rb_define_method(cRegChannel, "floodserv?", RegChannel_FloodServ, 0);
  rb_define_method(cRegChannel, "floodserv=", RegChannel_FloodServSet, 1);

  /* DB Methods */
  rb_define_method(cRegChannel, "save!", RegChannel_Save, 0);
  /* Class method */
  rb_define_method(cRegChannel, "RegChannel.by_name?", RegChannel_ByName, 1);
}

struct RegChannel*
rb_rbregchan2cregchan(VALUE self)
{
  struct RegChannel* out;
  VALUE channel = rb_iv_get(self, "@realptr");
  Data_Get_Struct(channel, struct RegChannel, out);
  return out;
}

VALUE
rb_cregchan2rbregchan(struct RegChannel *channel)
{
  VALUE rbchannel, real_channel;

  rbchannel = Data_Wrap_Struct(rb_cObject, 0, 0, channel);
  real_channel = do_ruby_ret(cRegChannel, rb_intern("new"), 1, rbchannel);

  if(real_channel == Qnil)
  {
    ilog(L_DEBUG, "RUBY ERROR: Ruby Failed To Create RegChannelStruct");
    return Qnil;
  }

  rb_gc_register_address(&real_channel);
  rb_gc_register_address(&rbchannel);

  return real_channel;
}

