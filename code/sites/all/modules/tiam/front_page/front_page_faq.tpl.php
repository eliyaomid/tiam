<div class="panel-group" id="accordion" role="tablist" aria-multiselectable="true">
  <?php $index_counter = 0; foreach ($items as $item): ?>
  <div class="panel panel-default">
    <div class="panel-heading" role="tab" id="heading<?php echo $index_counter;?>">
      <h4 class="panel-title">
        <a role="button" data-toggle="collapse" data-parent="#accordion" href="#collapse<?php echo $index_counter;?>" aria-expanded="true" aria-controls="collapseOne">
          <?php echo $item['title'];?>
        </a>
      </h4>
    </div>
    <div id="collapse<?php echo $index_counter;?>" class="panel-collapse collapse in" role="tabpanel" aria-labelledby="heading<?php echo $index_counter;?>">
      <div class="panel-body">
        <?php echo $item['body']; $index_counter++;?>
      </div>
    </div>
  </div>
  <?php endforeach; ?>
</div>